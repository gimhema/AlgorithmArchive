# 02. 중급 — 추상화, 메모리 관리, 함수형 스타일

> 목표: C++의 상속/템플릿/스마트 포인터/람다/STL 알고리즘으로 하던 일을 Rust 방식으로 옮길 수 있다.

---

## 1. ⭐ Trait — 인터페이스 / 추상 클래스 / concept 대체

> **C++ 고민:** "공통 인터페이스를 위해 `virtual` 함수를 가진 베이스 클래스를 만들었는데, Rust는 상속이 없다면서?"
> → **trait**으로 "할 수 있는 일"을 정의하고, 타입마다 `impl Trait for Type`으로 구현합니다.

```cpp
// C++
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual std::string name() const { return "shape"; }   // 기본 구현
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
    std::string name() const override { return "circle"; }
private:
    double r_;
};
```

```rust
// Rust
trait Shape {
    fn area(&self) -> f64;                       // 순수 가상 함수
    fn name(&self) -> String { "shape".into() }  // 기본 구현
}

struct Circle { r: f64 }
struct Square { s: f64 }

impl Shape for Circle {
    fn area(&self) -> f64 { 3.14159 * self.r * self.r }
    fn name(&self) -> String { "circle".into() }
}

impl Shape for Square {
    fn area(&self) -> f64 { self.s * self.s }
    // name()은 기본 구현 사용
}
```

### 상속이 없으면 코드 재사용은?
- **동작 공유** → trait의 기본 구현(default method)
- **데이터 공유** → 컴포지션 (멤버로 포함)
- **"is-a" 계층** → trait 상속(supertrait)

```rust
trait Named { fn name(&self) -> String; }
trait Animal: Named {             // Animal을 구현하려면 Named도 구현해야 함
    fn speak(&self) -> String {
        format!("{} makes a sound", self.name())
    }
}
```

### 외부 타입에도 trait 구현 가능
C++에서는 이미 존재하는 클래스(예: `std::string`)에 멤버 함수를 추가할 수 없지만, Rust는 내가 정의한 trait을 표준 타입에 구현할 수 있습니다.

```rust
trait Shout { fn shout(&self) -> String; }
impl Shout for str {
    fn shout(&self) -> String { self.to_uppercase() + "!" }
}
"hello".shout();  // "HELLO!"
```

> **고아 규칙(orphan rule):** trait 또는 타입 중 하나는 내 크레이트 소속이어야 합니다. (`impl Display for Vec<T>` 같은 건 불가 → newtype 패턴 사용, 고급 문서 참고)

---

## 2. ⭐ 제네릭 — 템플릿 대체

> **C++ 고민:** "템플릿 에러 메시지가 수백 줄… concept를 쓰자니 C++20 필요"
> → Rust 제네릭은 **처음부터 trait bound(=concept)가 필수**라서 에러가 정의부에서 명확하게 납니다.

```cpp
// C++20
template <typename T>
requires std::totally_ordered<T>
T max_of(const std::vector<T>& v) {
    T m = v[0];
    for (const auto& x : v) if (x > m) m = x;
    return m;
}
```

```rust
// Rust
fn max_of<T: PartialOrd + Copy>(v: &[T]) -> T {
    let mut m = v[0];
    for &x in v {
        if x > m { m = x; }
    }
    m
}

// bound가 길면 where 절
fn print_all<T, U>(a: T, b: U)
where
    T: std::fmt::Display,
    U: std::fmt::Debug + Clone,
{
    println!("{a} {:?}", b.clone());
}

// impl Trait 축약 문법 (auto 파라미터와 비슷)
fn total_area(shapes: &[impl Shape]) -> f64 {
    shapes.iter().map(|s| s.area()).sum()
}
```

### 제네릭 struct + 조건부 impl

```rust
struct Stack<T> {
    items: Vec<T>,
}

impl<T> Stack<T> {
    fn new() -> Self { Self { items: Vec::new() } }
    fn push(&mut self, x: T) { self.items.push(x); }
    fn pop(&mut self) -> Option<T> { self.items.pop() }
    fn peek(&self) -> Option<&T> { self.items.last() }
}

// T가 Display일 때만 존재하는 메서드 (C++의 requires 멤버 함수)
impl<T: std::fmt::Display> Stack<T> {
    fn dump(&self) {
        for x in &self.items { println!("{x}"); }
    }
}
```

> 템플릿 특수화(specialization)는 stable Rust에 없습니다. 대신 trait을 타입별로 구현하는 방식으로 해결합니다.

---

## 3. 정적 디스패치 vs 동적 디스패치

| C++ | Rust | 방식 |
|-----|------|------|
| 템플릿 `template<class T> void f(T&)` | `fn f<T: Shape>(x: &T)` / `fn f(x: &impl Shape)` | 정적 (monomorphization, 인라인 가능) |
| `void f(const Shape&)` (virtual) | `fn f(x: &dyn Shape)` | 동적 (vtable) |
| `std::vector<std::unique_ptr<Shape>>` | `Vec<Box<dyn Shape>>` | 이종 컬렉션 |

```rust
// 이종 컬렉션 (다형성)
let shapes: Vec<Box<dyn Shape>> = vec![
    Box::new(Circle { r: 1.0 }),
    Box::new(Square { s: 2.0 }),
];
for s in &shapes {
    println!("{} {}", s.name(), s.area());
}

fn describe(s: &dyn Shape) { println!("{}", s.area()); }
```

### 선택 기준
- 타입 종류가 **닫혀 있고** 알고 있다 → `enum` + `match` (가장 빠르고 간단)
- 타입이 **열려 있고**(플러그인 등) 런타임에 섞인다 → `dyn Trait`
- 성능이 중요하고 컴파일 타임에 타입이 정해진다 → 제네릭

> **Object safety:** `dyn Trait`으로 쓰려면 trait 메서드가 제네릭 파라미터를 가지거나 `Self`를 값으로 반환하면 안 됩니다 (가상 템플릿 함수가 불가능한 것과 같은 이유).

### 다운캐스팅 (`dynamic_cast` 대체)
가능은 하지만(`std::any::Any`) 권장되지 않습니다. 필요하다면 설계를 enum으로 바꾸는 걸 먼저 고려하세요.

```rust
use std::any::Any;
fn try_circle(x: &dyn Any) {
    if let Some(c) = x.downcast_ref::<Circle>() { println!("r={}", c.r); }
}
```

---

## 4. 자주 구현하는 표준 trait (특수 멤버 함수 / 연산자 대체)

| C++ | Rust trait | 비고 |
|-----|-----------|------|
| 복사 생성자 | `Clone` | `#[derive(Clone)]` |
| trivially copyable | `Copy` | `Clone`도 필요 |
| 기본 생성자 | `Default` | |
| 소멸자 | `Drop` | |
| `operator==` | `PartialEq`, `Eq` | |
| `operator<` / `<=>` | `PartialOrd`, `Ord` | |
| `std::hash<T>` 특수화 | `Hash` | HashMap 키로 쓰려면 `Eq + Hash` |
| `operator<<` (ostream) | `Display` (사용자용), `Debug` (개발용) | |
| 변환 생성자 / 변환 연산자 | `From` / `Into` | |
| `operator+` 등 | `Add`, `Sub`, `Mul`, `Neg`, `AddAssign` ... | `std::ops` |
| `operator[]` | `Index`, `IndexMut` | |
| `operator*` / `operator->` | `Deref`, `DerefMut` | 스마트 포인터 구현용 |
| `begin()/end()` | `IntoIterator` / `Iterator` | |

```rust
use std::fmt;
use std::ops::Add;

#[derive(Debug, Clone, Copy, PartialEq, Default)]
struct Vec2 { x: f64, y: f64 }

impl Add for Vec2 {
    type Output = Vec2;
    fn add(self, o: Vec2) -> Vec2 { Vec2 { x: self.x + o.x, y: self.y + o.y } }
}

impl fmt::Display for Vec2 {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "({:.1}, {:.1})", self.x, self.y)
    }
}

impl From<(f64, f64)> for Vec2 {
    fn from((x, y): (f64, f64)) -> Self { Vec2 { x, y } }
}

let a = Vec2 { x: 1.0, y: 2.0 };
let b: Vec2 = (3.0, 4.0).into();   // From을 구현하면 Into는 자동
println!("{}", a + b);              // (4.0, 6.0)
println!("{}", a.to_string());      // Display 구현 시 to_string() 자동
```

### HashMap 키 / 정렬 가능한 구조체

```rust
#[derive(Debug, Clone, PartialEq, Eq, Hash, PartialOrd, Ord)]
struct Pos { r: i32, c: i32 }      // 필드 순서대로 사전식 비교
```

> `f64`는 NaN 때문에 `Eq`, `Ord`, `Hash`가 없습니다. 정렬은 `v.sort_by(|a, b| a.partial_cmp(b).unwrap())` 또는 `a.total_cmp(b)`를 사용합니다.

---

## 5. ⭐ RAII와 `Drop`

C++의 RAII 철학은 Rust에서도 그대로입니다. 차이점: move된 객체는 소멸자가 호출되지 않아 "moved-from 상태" 처리가 필요 없습니다.

```rust
struct FileGuard { name: String }

impl Drop for FileGuard {
    fn drop(&mut self) {
        println!("closing {}", self.name);
    }
}

{
    let _g = FileGuard { name: "a.txt".into() };
    // ...
}   // 여기서 "closing a.txt"

let g = FileGuard { name: "b.txt".into() };
drop(g);   // 명시적으로 조기 해제 (std::mem::drop)
```

> **함정:** `let _ = FileGuard{..};` 는 **즉시** drop 됩니다. 스코프 끝까지 유지하려면 `let _g = ...` 처럼 이름을 붙이세요 (C++에서 `std::lock_guard<std::mutex>(m);` 처럼 이름 없는 임시 객체 실수와 동일).

---

## 6. ⭐ 스마트 포인터

| C++ | Rust | 용도 |
|-----|------|------|
| `std::unique_ptr<T>` | `Box<T>` | 힙 할당, 단일 소유. 재귀 타입, trait object |
| `std::shared_ptr<T>` | `Rc<T>` | 공유 소유 (단일 스레드, 비원자적 카운트 → 더 빠름) |
| `std::shared_ptr<T>` (스레드 간) | `Arc<T>` | 원자적 참조 카운트 |
| `std::weak_ptr<T>` | `Weak<T>` | 순환 참조 끊기 |
| `mutable` 멤버 / const_cast | `Cell<T>`, `RefCell<T>` | 내부 가변성 |

### Box — 재귀 타입

```rust
enum List {
    Cons(i32, Box<List>),
    Nil,
}

// 트리
#[derive(Debug)]
struct Node {
    val: i32,
    left: Option<Box<Node>>,    // nullable unique_ptr
    right: Option<Box<Node>>,
}
```

### Rc + RefCell — 공유 + 수정

> **C++ 고민:** "`shared_ptr<T>`로 여러 곳에서 공유하면서 수정도 했는데?"
> → `Rc<T>`는 불변 공유만 허용합니다. 수정하려면 `Rc<RefCell<T>>` — 빌림 규칙을 **런타임에** 검사합니다.

```rust
use std::rc::Rc;
use std::cell::RefCell;

#[derive(Debug)]
struct Account { balance: i64 }

let shared = Rc::new(RefCell::new(Account { balance: 100 }));
let a = Rc::clone(&shared);     // 참조 카운트 증가 (shared_ptr 복사)
let b = Rc::clone(&shared);

a.borrow_mut().balance += 50;   // 가변 빌림 (동시에 두 개면 panic)
println!("{}", b.borrow().balance);          // 150
println!("refs = {}", Rc::strong_count(&shared)); // 3
```

### Weak — 부모 포인터

```rust
use std::rc::{Rc, Weak};
use std::cell::RefCell;

struct TreeNode {
    val: i32,
    parent: RefCell<Weak<TreeNode>>,
    children: RefCell<Vec<Rc<TreeNode>>>,
}

let parent = Rc::new(TreeNode { val: 1, parent: RefCell::new(Weak::new()), children: RefCell::new(vec![]) });
let child  = Rc::new(TreeNode { val: 2, parent: RefCell::new(Weak::new()), children: RefCell::new(vec![]) });
*child.parent.borrow_mut() = Rc::downgrade(&parent);
parent.children.borrow_mut().push(Rc::clone(&child));

if let Some(p) = child.parent.borrow().upgrade() {   // weak_ptr::lock()
    println!("parent = {}", p.val);
}
```

> **실무 팁:** 그래프/트리를 `Rc<RefCell<..>>`로 만들면 코드가 복잡해집니다. **`Vec<Node>` + 인덱스(usize)** 방식(arena)이 Rust에서 훨씬 흔하고 빠릅니다 (고급 문서 참고).

### Cell — Copy 타입의 내부 가변성

```rust
use std::cell::Cell;
struct Counter { hits: Cell<u32> }
impl Counter {
    fn touch(&self) { self.hits.set(self.hits.get() + 1); }  // &self 인데 수정 가능
}
```

---

## 7. ⭐ 클로저 — 람다 대체

```cpp
// C++
int base = 10;
auto add  = [base](int x) { return x + base; };   // 값 캡처
auto inc  = [&base]() { base++; };                // 참조 캡처
auto take = [s = std::move(str)]() { ... };       // move 캡처
```

```rust
// Rust: 캡처 방식은 사용법에 따라 자동 추론
let mut base = 10;
let add = |x: i32| x + base;              // 불변 빌림으로 캡처
println!("{}", add(5));

let mut inc = || base += 1;               // 가변 빌림 캡처 (클로저 자체도 mut)
inc();

let s = String::from("hi");
let take = move || println!("{s}");       // move: 소유권을 클로저로 이동
take();
```

### 클로저를 인자로 받기 / 반환하기

| Rust trait | 의미 | 호출 횟수 |
|-----------|------|-----------|
| `Fn` | 캡처를 `&`로 사용 | 여러 번 |
| `FnMut` | 캡처를 `&mut`로 사용 | 여러 번 |
| `FnOnce` | 캡처를 소비 | 한 번 |

```rust
// 템플릿 방식 (정적, 인라인됨) — template<class F> void apply(F f)
fn apply<F: Fn(i32) -> i32>(f: F, x: i32) -> i32 { f(x) }

// std::function 방식 (동적)
fn make_adder(n: i32) -> Box<dyn Fn(i32) -> i32> {
    Box::new(move |x| x + n)
}

// impl Trait 반환 (힙 할당 없음)
fn make_multiplier(n: i32) -> impl Fn(i32) -> i32 {
    move |x| x * n
}

// 콜백 저장 (std::vector<std::function<void()>>)
struct EventBus {
    handlers: Vec<Box<dyn Fn(&str)>>,
}
impl EventBus {
    fn on(&mut self, f: impl Fn(&str) + 'static) { self.handlers.push(Box::new(f)); }
    fn emit(&self, msg: &str) { for h in &self.handlers { h(msg); } }
}
```

---

## 8. ⭐ Iterator — `<algorithm>` / ranges 대체

> **C++ 고민:** "`std::transform`, `std::copy_if`, `std::accumulate` 조합이 장황했는데…"
> → Rust iterator 체인은 C++20 ranges와 비슷하며, **지연 평가 + 제로 코스트**입니다.

```rust
let v = vec![1, 2, 3, 4, 5, 6];

// iter()      → &T 순회        (const auto&)
// iter_mut()  → &mut T 순회    (auto&)
// into_iter() → T 순회 (소비)  (move)

let evens_squared: Vec<i32> = v.iter()
    .filter(|&&x| x % 2 == 0)     // copy_if
    .map(|&x| x * x)              // transform
    .collect();                   // 결과 컨테이너로 수집

let sum: i32 = v.iter().sum();                         // accumulate
let product: i32 = v.iter().product();
let max = v.iter().max();                              // Option<&i32>
let pos = v.iter().position(|&x| x == 4);              // find → 인덱스
let any = v.iter().any(|&x| x > 5);                    // any_of
let all = v.iter().all(|&x| x > 0);                    // all_of
let cnt = v.iter().filter(|&&x| x > 2).count();        // count_if
let folded = v.iter().fold(0, |acc, x| acc + x);       // accumulate with init
let first_big = v.iter().find(|&&x| x > 3);            // find_if

// 자주 쓰는 어댑터
v.iter().enumerate();              // (index, &x)
v.iter().zip(other.iter());        // 두 시퀀스 묶기
v.iter().rev();                    // 역순
v.iter().skip(2).take(3);          // 부분
v.iter().step_by(2);
v.chunks(2);                       // [1,2], [3,4], [5,6]
v.windows(3);                      // 슬라이딩 윈도우
v.iter().flat_map(|x| vec![*x; 2]);
v.iter().cloned();                 // &T → T (Clone)
v.iter().copied();                 // &T → T (Copy)
v.iter().min_by_key(|x| (*x - 3).abs());
v.iter().max_by(|a, b| a.cmp(b));

// collect는 타입에 따라 다양한 컨테이너로
use std::collections::{HashMap, HashSet};
let set: HashSet<i32> = v.iter().copied().collect();
let map: HashMap<i32, String> = v.iter().map(|&x| (x, x.to_string())).collect();
let joined: String = vec!["a", "b", "c"].join(",");
let s: String = vec!['a', 'b'].iter().collect();

// Result 컬렉션 → Result<Vec>  (하나라도 실패하면 Err)
let nums: Result<Vec<i32>, _> = ["1", "2", "x"].iter().map(|s| s.parse::<i32>()).collect();
```

### 직접 Iterator 구현

```rust
struct Fib { a: u64, b: u64 }

impl Iterator for Fib {
    type Item = u64;
    fn next(&mut self) -> Option<u64> {
        let r = self.a;
        self.a = self.b;
        self.b += r;
        Some(r)
    }
}

let first10: Vec<u64> = Fib { a: 0, b: 1 }.take(10).collect();
```

`Iterator`를 구현하면 `map`, `filter`, `sum` 등 모든 어댑터를 공짜로 쓸 수 있습니다.

---

## 9. ⭐ 실무 에러 처리: 커스텀 에러, `thiserror`, `anyhow`

### 표준만으로 커스텀 에러 만들기

```rust
use std::fmt;

#[derive(Debug)]
enum ConfigError {
    Io(std::io::Error),
    Parse { line: usize, msg: String },
    Missing(&'static str),
}

impl fmt::Display for ConfigError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            ConfigError::Io(e) => write!(f, "io error: {e}"),
            ConfigError::Parse { line, msg } => write!(f, "line {line}: {msg}"),
            ConfigError::Missing(k) => write!(f, "missing key: {k}"),
        }
    }
}
impl std::error::Error for ConfigError {}

// ? 에서 자동 변환되도록 From 구현
impl From<std::io::Error> for ConfigError {
    fn from(e: std::io::Error) -> Self { ConfigError::Io(e) }
}

fn load(path: &str) -> Result<String, ConfigError> {
    let s = std::fs::read_to_string(path)?;   // io::Error → ConfigError 자동 변환
    if s.is_empty() { return Err(ConfigError::Missing("body")); }
    Ok(s)
}
```

### 실무 표준 조합
- **라이브러리**: `thiserror` — 위의 보일러플레이트를 derive로 생성
- **애플리케이션**: `anyhow` — 어떤 에러든 담는 `anyhow::Result`, 컨텍스트 추가

```rust
// 라이브러리 쪽
use thiserror::Error;

#[derive(Debug, Error)]
pub enum DbError {
    #[error("connection failed: {0}")]
    Connection(String),
    #[error("not found: id={id}")]
    NotFound { id: u64 },
    #[error(transparent)]
    Io(#[from] std::io::Error),
}
```

```rust
// 애플리케이션 쪽
use anyhow::{Context, Result, bail};

fn run() -> Result<()> {
    let cfg = std::fs::read_to_string("app.toml")
        .context("failed to read app.toml")?;     // 에러에 설명 추가 (스택처럼 쌓임)
    if cfg.is_empty() {
        bail!("config is empty");                 // return Err(anyhow!(...))
    }
    Ok(())
}
```

### Option ↔ Result 변환

```rust
let v: Option<i32> = Some(3);
let r: Result<i32, &str> = v.ok_or("none!");   // Option → Result
let o: Option<i32> = r.ok();                   // Result → Option

// Option을 반환하는 함수 안에서도 ? 사용 가능
fn first_char_upper(s: &str) -> Option<char> {
    let c = s.chars().next()?;
    Some(c.to_ascii_uppercase())
}
```

---

## 10. 라이프타임 기초

> **C++ 고민:** "함수가 인자 중 하나의 참조를 반환하면, 반환값이 얼마나 유효한지 주석으로만 적었었는데…"
> → Rust는 그걸 **타입에 적습니다.** 대부분은 컴파일러가 추론(elision)하고, 모호할 때만 직접 적습니다.

```rust
// 반환된 참조는 a, b 둘 다보다 오래 살 수 없다
fn longer<'a>(a: &'a str, b: &'a str) -> &'a str {
    if a.len() > b.len() { a } else { b }
}

// 생략 규칙: 입력 참조가 하나거나 &self 메서드면 자동 추론
fn first_word(s: &str) -> &str { s.split(' ').next().unwrap_or("") }
```

### 참조를 멤버로 가진 struct

```rust
// C++: struct Parser { std::string_view input; };  — input 수명은 프로그래머 책임
struct Parser<'a> {
    input: &'a str,
    pos: usize,
}

impl<'a> Parser<'a> {
    fn new(input: &'a str) -> Self { Self { input, pos: 0 } }
    fn next_token(&mut self) -> Option<&'a str> {
        let rest = &self.input[self.pos..];
        let rest_trim = rest.trim_start();
        if rest_trim.is_empty() { return None; }
        let start = self.pos + (rest.len() - rest_trim.len());
        let end = rest_trim.find(' ').map(|i| start + i).unwrap_or(self.input.len());
        self.pos = end;
        Some(&self.input[start..end])
    }
}
```

> **실무 팁:** struct가 참조를 가지면 라이프타임이 전염됩니다. 성능상 꼭 필요한 게 아니라면 **struct는 소유 타입(`String`, `Vec`)을 갖게 하는 것**이 훨씬 편합니다.

`'static`: 프로그램 전체 기간 동안 유효 (문자열 리터럴, 전역). `T: 'static` 바운드는 "빌린 참조를 포함하지 않는 타입"이라는 의미로 스레드/콜백 저장에서 자주 등장합니다.

---

## 11. 테스트

gtest 같은 외부 프레임워크가 필요 없습니다.

```rust
pub fn add(a: i32, b: i32) -> i32 { a + b }

#[cfg(test)]           // 테스트 빌드에서만 컴파일
mod tests {
    use super::*;

    #[test]
    fn test_add() {
        assert_eq!(add(2, 3), 5);
    }

    #[test]
    #[should_panic(expected = "overflow")]
    fn test_panic() {
        panic!("overflow");
    }

    #[test]
    fn test_result() -> Result<(), String> {
        if add(1, 1) == 2 { Ok(()) } else { Err("bad".into()) }
    }
}
```

```bash
cargo test                 # 전체
cargo test add             # 이름에 add가 포함된 테스트만
cargo test -- --nocapture  # println 출력 보기
```

- `tests/` 디렉터리: 통합 테스트 (공개 API만 사용)
- 문서 주석(`///`) 안의 코드 블록도 테스트로 실행됨 (doc test)

```rust
/// 두 수를 더합니다.
///
/// ```
/// assert_eq!(mycrate::add(1, 2), 3);
/// ```
pub fn add(a: i32, b: i32) -> i32 { a + b }
```

---

## 12. 자주 쓰는 crate (실무 생태계)

| 용도 | crate | C++ 대응 |
|------|-------|----------|
| 직렬화 | `serde`, `serde_json`, `toml` | nlohmann/json |
| CLI 파싱 | `clap` | boost::program_options, CLI11 |
| 에러 | `thiserror`, `anyhow` | |
| 로깅 | `tracing`, `log` + `env_logger` | spdlog |
| 비동기 런타임 | `tokio` | boost::asio |
| HTTP | `reqwest`(클라), `axum`(서버) | cpr, Beast |
| 병렬 처리 | `rayon` | TBB, OpenMP |
| 정규식 | `regex` | std::regex (훨씬 빠름) |
| 날짜/시간 | `chrono`, `time` | std::chrono |
| 랜덤 | `rand` | `<random>` |
| 벤치마크 | `criterion` | Google Benchmark |
| 전역 지연 초기화 | `std::sync::OnceLock` / `LazyLock` | 함수 static 지역 변수 |

```rust
// serde 예시
use serde::{Deserialize, Serialize};

#[derive(Debug, Serialize, Deserialize)]
struct User {
    id: u64,
    name: String,
    #[serde(default)]
    tags: Vec<String>,
}

let json = r#"{"id": 1, "name": "kim"}"#;
let u: User = serde_json::from_str(json)?;
let out = serde_json::to_string_pretty(&u)?;
```

```rust
// clap 예시
use clap::Parser;

#[derive(Parser, Debug)]
#[command(version, about)]
struct Args {
    /// 입력 파일
    input: String,
    /// 반복 횟수
    #[arg(short, long, default_value_t = 1)]
    count: u32,
    #[arg(short, long)]
    verbose: bool,
}

fn main() {
    let args = Args::parse();
    println!("{args:?}");
}
```

---

## 중급 체크리스트

- [ ] 가상 함수 기반 설계를 trait + `dyn Trait` / 제네릭 / enum 중 적절히 선택해 옮길 수 있다
- [ ] `derive`로 `Debug, Clone, PartialEq, Hash, Default` 등을 붙이는 이유를 안다
- [ ] `Box`, `Rc`, `Arc`, `RefCell`, `Weak`의 용도를 구분한다
- [ ] iterator 체인으로 STL 알고리즘 코드를 대체할 수 있다
- [ ] `Fn` / `FnMut` / `FnOnce`의 차이를 안다
- [ ] `thiserror` / `anyhow`로 에러 타입을 설계할 수 있다
- [ ] 라이프타임 표기 `'a`가 무엇을 의미하는지 읽을 수 있다

다음: [03_Advanced.md](03_Advanced.md)
