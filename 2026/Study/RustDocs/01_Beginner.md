# 01. 초급 — C++ 개발자를 위한 Rust 기본기

> 목표: Rust 코드를 읽고, 간단한 프로그램을 작성하며, **소유권/빌림** 개념을 C++과 비교해 이해한다.

---

## 1. 개발 환경 & Cargo

C++에서는 컴파일러(gcc/clang) + 빌드 시스템(CMake) + 패키지 매니저(vcpkg/conan) + 테스트 프레임워크(gtest)를 따로 골라야 했지만, Rust는 **Cargo** 하나로 해결됩니다.

```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh   # 설치
cargo new hello          # 바이너리 프로젝트 생성 (--lib 이면 라이브러리)
cd hello
cargo build              # 디버그 빌드 (target/debug)
cargo build --release    # 최적화 빌드 (-O3 에 해당)
cargo run                # 빌드 + 실행
cargo test               # 테스트 실행
cargo check              # 코드 생성 없이 타입 검사만 (가장 빠름, 자주 사용)
cargo fmt                # clang-format 역할
cargo clippy             # clang-tidy 역할 (린터)
cargo doc --open         # Doxygen 역할
```

`Cargo.toml` (CMakeLists.txt + conanfile 역할):

```toml
[package]
name = "hello"
version = "0.1.0"
edition = "2021"

[dependencies]
serde = { version = "1", features = ["derive"] }
anyhow = "1"
```

---

## 2. 변수와 가변성

> **C++ 고민:** "나는 `const`를 습관적으로 붙이는데, Rust에서는?"
> → Rust는 **기본이 const** 입니다. 바꾸고 싶을 때만 `mut`를 붙입니다.

```cpp
// C++
const int a = 10;
int b = 20;
b += 1;
constexpr int MAX = 100;
```

```rust
// Rust
let a = 10;          // 불변 (const int)
let mut b = 20;      // 가변
b += 1;
const MAX: i32 = 100;          // 컴파일 타임 상수 (constexpr). 타입 명시 필수
static GREETING: &str = "hi";  // 전역 static
```

### 섀도잉 (Shadowing)
같은 이름으로 다시 `let`을 선언할 수 있습니다. 타입 변환 후 이름을 재사용할 때 유용합니다.

```rust
let input = "42";
let input: i32 = input.parse().unwrap();   // 문자열 → 정수, 이름 재사용
```

---

## 3. 기본 타입

| C++ | Rust |
|-----|------|
| `int8_t` / `uint8_t` | `i8` / `u8` |
| `int32_t` / `uint32_t` | `i32` / `u32` |
| `int64_t` / `uint64_t` | `i64` / `u64` |
| `size_t` / `ptrdiff_t` | `usize` / `isize` |
| `float` / `double` | `f32` / `f64` |
| `bool` | `bool` |
| `char` (1바이트) | `u8` — Rust의 `char`는 4바이트 유니코드 스칼라 |
| `std::tuple<int, double>` | `(i32, f64)` |
| `int arr[3]` / `std::array<int,3>` | `[i32; 3]` |

### 암묵적 형변환이 없다

```cpp
// C++: 암묵적 변환 허용 (버그의 원천)
int a = 10;
double b = a;      // OK
size_t n = -1;     // 경고만, 컴파일 됨
```

```rust
// Rust: 모든 수치 변환은 명시적
let a: i32 = 10;
let b: f64 = a as f64;              // as 캐스팅 (static_cast, 잘림 가능)
let c: u8 = u8::try_from(300i32).unwrap_or(255); // 안전한 변환 (실패 시 Err)
let d: i64 = a.into();             // 손실 없는 변환은 into()
```

### 정수 오버플로
- 디버그 빌드: **panic** (프로그램 중단)
- 릴리즈 빌드: wrap-around
- 의도를 명시하려면: `wrapping_add`, `checked_add`, `saturating_add`, `overflowing_add`

```rust
let x: u8 = 250;
let y = x.checked_add(10);     // None
let z = x.saturating_add(10);  // 255
let w = x.wrapping_add(10);    // 4
```

---

## 4. 함수와 표현식

```rust
fn add(a: i32, b: i32) -> i32 {
    a + b          // 세미콜론 없는 마지막 표현식이 반환값
}

fn early(x: i32) -> i32 {
    if x < 0 {
        return 0;  // 조기 반환은 return 사용
    }
    x * 2
}
```

Rust에서는 `if`, `match`, `{}` 블록이 **모두 값을 가지는 표현식**입니다. C++의 삼항 연산자는 없지만 `if`로 대체됩니다.

```cpp
// C++
int v = cond ? 1 : 2;
```

```rust
// Rust
let v = if cond { 1 } else { 2 };

let result = {
    let tmp = compute();
    tmp * 2            // 블록의 값
};
```

---

## 5. 제어 흐름

```rust
// for: 범위 기반 for 문만 존재 (C 스타일 for 없음)
for i in 0..10 { }          // 0..9   (for (int i=0; i<10; ++i))
for i in 0..=10 { }         // 0..10 (포함)
for i in (0..10).rev() { }  // 역순
for i in (0..10).step_by(2) { }

let mut v = vec![1, 2, 3];
for x in &v { }              // for (const auto& x : v)
for x in &mut v { *x += 1; } // for (auto& x : v)
for (i, x) in v.iter().enumerate() { } // 인덱스 + 값
for x in v { }               // for (auto x : std::move(v)) — v를 소비(move)함

// while
let mut n = 0;
while n < 10 { n += 1; }

// 무한 루프 (while(true)) — break로 값을 반환할 수도 있음
let found = loop {
    n += 1;
    if n == 42 { break n; }
};

// 중첩 루프 탈출: 라벨 (C++의 goto 대체)
'outer: for i in 0..10 {
    for j in 0..10 {
        if i * j == 42 { break 'outer; }
    }
}
```

---

## 6. ⭐ 소유권 (Ownership) — 가장 중요한 개념

> **C++ 고민:** "`std::move` 후에 원본 객체를 실수로 쓰는 버그, 댕글링 포인터… 이걸 어떻게 막지?"
> → Rust는 **컴파일러가 막아줍니다.**

### 규칙 3가지
1. 모든 값은 **단 하나의 소유자**(변수)를 가진다.
2. 소유자가 스코프를 벗어나면 값은 해제된다 (RAII — C++과 동일).
3. 대입/함수 인자 전달 시 **기본 동작이 move** 이다 (C++은 기본이 copy).

```cpp
// C++
std::string a = "hello";
std::string b = a;              // 복사 (깊은 복사)
std::string c = std::move(a);   // 이동, a는 "유효하지만 미지정 상태"
std::cout << a;                 // 컴파일 됨! (논리 버그)
```

```rust
// Rust
let a = String::from("hello");
let b = a.clone();     // 복사는 명시적으로 clone()
let c = a;             // 이동 (기본)
// println!("{}", a);  // ❌ 컴파일 에러: value borrowed here after move
```

### Copy 타입
정수, 부동소수, bool, char, 그리고 이들로만 이루어진 튜플/배열은 `Copy` 타입이라 move 대신 비트 복사됩니다 (C++의 trivially copyable).

```rust
let x = 5;
let y = x;      // 복사
println!("{x} {y}");  // OK
```

### 함수 인자도 move

```rust
fn consume(s: String) { println!("{s}"); }  // s의 소유권을 가져감 → 함수 끝에서 해제

let s = String::from("hi");
consume(s);
// consume(s);  // ❌ 이미 move 됨
```

C++로 보면 `void consume(std::string s)`에 `std::move(s)`로 넘긴 것과 같은데, 이후 사용을 컴파일러가 금지합니다.

---

## 7. ⭐ 빌림 (Borrowing) — 참조

> **C++ 고민:** "복사 비용 없이 넘기려면 `const T&`, 수정하려면 `T&`를 썼는데?"
> → 그대로 `&T`, `&mut T` 입니다. 단, **규칙이 강제**됩니다.

| C++ | Rust |
|-----|------|
| `void f(const std::string& s)` | `fn f(s: &String)` → 보통 `fn f(s: &str)` |
| `void f(std::string& s)` | `fn f(s: &mut String)` |
| `void f(std::string s)` (move로 받음) | `fn f(s: String)` |

```rust
fn length(s: &str) -> usize { s.len() }
fn append(s: &mut String) { s.push_str(" world"); }

let mut s = String::from("hello");
let n = length(&s);     // 호출부에서도 & 를 명시 → 가독성 ↑
append(&mut s);         // 수정된다는 사실이 호출부에서 보임
```

### 빌림 규칙 (Borrow Checker)
어떤 시점이든 다음 중 **하나만** 가능:
- 불변 참조 `&T` **여러 개**, 또는
- 가변 참조 `&mut T` **딱 하나**

이 규칙 덕분에 **iterator invalidation**, **data race**가 컴파일 타임에 차단됩니다.

```cpp
// C++: 런타임 UB (iterator invalidation)
std::vector<int> v = {1, 2, 3};
for (auto& x : v) {
    if (x == 2) v.push_back(4);   // 💥 재할당 시 x가 댕글링
}
```

```rust
// Rust: 컴파일 에러
let mut v = vec![1, 2, 3];
for x in &v {
    if *x == 2 {
        // v.push(4);  // ❌ cannot borrow `v` as mutable because it is also borrowed as immutable
    }
}
// 해결: 먼저 수집 후 추가
let extra: Vec<i32> = v.iter().filter(|&&x| x == 2).map(|_| 4).collect();
v.extend(extra);
```

### 댕글링 참조 방지

```cpp
// C++: 컴파일 됨 (경고 정도), 런타임 UB
const std::string& bad() {
    std::string s = "hi";
    return s;
}
```

```rust
// Rust: 컴파일 에러
// fn bad() -> &String {
//     let s = String::from("hi");
//     &s    // ❌ s는 함수 끝에서 해제됨
// }
fn good() -> String {
    String::from("hi")   // 소유권을 넘겨줌 (RVO처럼 비용 거의 없음)
}
```

---

## 8. 문자열: `String` vs `&str`

| C++ | Rust | 설명 |
|-----|------|------|
| `std::string` | `String` | 힙 소유, 가변 |
| `std::string_view` | `&str` | 빌린 문자열 슬라이스 |
| `"literal"` (`const char*`) | `"literal"` (`&'static str`) | |

```rust
let literal: &str = "hello";
let owned: String = String::from("hello");  // 또는 "hello".to_string(), "hello".to_owned()
let view: &str = &owned;                    // String → &str (자동 변환, Deref)
let sub: &str = &owned[1..3];               // "el" (바이트 인덱스!)

// 포맷팅 (std::format / printf 대체)
let name = "Kim";
let msg = format!("Hello, {name}! {} + {} = {}", 1, 2, 1 + 2);
println!("{msg}");
println!("{:>8.2}", 3.14159);  // 우측정렬, 폭 8, 소수 2자리
println!("{:?}", vec![1, 2]);  // Debug 출력
println!("{:#?}", some_struct); // 예쁜 Debug 출력

// 자주 쓰는 메서드
let s = "  a,b,c  ";
let parts: Vec<&str> = s.trim().split(',').collect();
let upper = s.to_uppercase();
let has = s.contains("b");
let replaced = s.replace(",", ";");
for c in "한글".chars() { }   // 유니코드 문자 단위 순회
for b in "abc".bytes() { }    // 바이트 단위
```

> **팁:** 함수 파라미터는 `&String` 대신 `&str`로 받으세요. `String`, `&str`, 리터럴 모두 받을 수 있습니다 (C++에서 `const std::string&` 대신 `std::string_view`를 쓰는 것과 같은 이유).

> **주의:** `s[0]` 같은 정수 인덱싱은 불가합니다 (UTF-8이라 의미가 모호). `s.chars().nth(0)` 또는 `s.as_bytes()[0]`을 사용합니다.

---

## 9. 컬렉션: `Vec`, 슬라이스, `HashMap`

```rust
// Vec<T> == std::vector<T>
let mut v: Vec<i32> = Vec::new();
let mut v2 = vec![1, 2, 3];
let zeros = vec![0; 10];              // std::vector<int>(10, 0)
v.push(10);                           // push_back
v.pop();                              // Option<T> 반환 (비어있으면 None)
let len = v2.len();
let first = v2[0];                    // 범위 밖이면 panic (at()처럼 검사함)
let maybe = v2.get(10);               // Option<&i32> — 안전한 접근
v2.sort();
v2.sort_by(|a, b| b.cmp(a));          // 내림차순
v2.sort_by_key(|x| x.abs());
v2.dedup();
v2.retain(|&x| x % 2 == 0);           // erase-remove idiom 대체
v2.insert(0, 100);
v2.remove(0);
v2.extend([4, 5, 6]);
let has = v2.contains(&4);
v2.reserve(100);

// 슬라이스 &[T] == std::span<T>
fn sum(xs: &[i32]) -> i32 { xs.iter().sum() }
sum(&v2);            // Vec → 슬라이스
sum(&v2[1..3]);      // 부분 슬라이스
sum(&[1, 2, 3]);     // 배열 → 슬라이스

// 2차원 벡터
let mut grid = vec![vec![0; 5]; 3];   // 3x5
grid[1][2] = 7;
```

```rust
use std::collections::HashMap;   // std::unordered_map
use std::collections::BTreeMap;  // std::map (정렬)
use std::collections::HashSet;   // std::unordered_set
use std::collections::VecDeque;  // std::deque
use std::collections::BinaryHeap; // std::priority_queue (max-heap)

let mut m: HashMap<String, i32> = HashMap::new();
m.insert("a".to_string(), 1);
if let Some(v) = m.get("a") { println!("{v}"); }   // find() != end() 패턴 대체

// ⭐ entry API: "없으면 삽입, 있으면 수정" — map[key]++ 대체
let text = "a b a c b a";
let mut count: HashMap<&str, usize> = HashMap::new();
for w in text.split_whitespace() {
    *count.entry(w).or_insert(0) += 1;
}

for (k, v) in &count { println!("{k}: {v}"); }
m.remove("a");
```

> C++의 `m[key]`는 키가 없으면 기본값을 **삽입**하지만, Rust의 `m[&key]`는 키가 없으면 **panic** 입니다. 실무에선 `get`, `entry`를 주로 씁니다.

---

## 10. 구조체와 메서드 (`class` 대체)

```cpp
// C++
class Rect {
public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area() const { return w_ * h_; }
    void scale(double k) { w_ *= k; h_ *= k; }
    static Rect square(double s) { return Rect(s, s); }
private:
    double w_, h_;
};
```

```rust
// Rust
#[derive(Debug, Clone, PartialEq)]   // 자동 구현: 디버그 출력, 복제, == 비교
pub struct Rect {
    w: f64,          // 기본 private (모듈 단위)
    h: f64,
}

impl Rect {
    // 생성자는 관례적으로 new 라는 연관 함수 (static 함수)
    pub fn new(w: f64, h: f64) -> Self {
        Self { w, h }
    }
    pub fn square(s: f64) -> Self {
        Self::new(s, s)
    }
    // &self   == const 멤버 함수
    pub fn area(&self) -> f64 {
        self.w * self.h
    }
    // &mut self == non-const 멤버 함수
    pub fn scale(&mut self, k: f64) {
        self.w *= k;
        self.h *= k;
    }
    // self (값) == 객체를 소비하는 함수 (&& 한정 멤버 함수와 유사)
    pub fn into_tuple(self) -> (f64, f64) {
        (self.w, self.h)
    }
}

let mut r = Rect::new(2.0, 3.0);
r.scale(2.0);
println!("{:?} area={}", r, r.area());
```

### 기타 struct 형태

```rust
struct Meters(f64);        // 튜플 구조체 (newtype 패턴에 자주 사용)
struct Marker;             // 유닛 구조체 (크기 0)

let p = Rect { w: 1.0, ..r.clone() };   // 나머지 필드는 r에서 가져옴
```

### 기본값: `Default`

```rust
#[derive(Debug, Default)]
struct Config {
    port: u16,          // 0
    host: String,       // ""
    verbose: bool,      // false
}
let c = Config { port: 8080, ..Default::default() };
```

---

## 11. ⭐ enum과 match (`enum class` + `std::variant` 의 상위호환)

> **C++ 고민:** "`std::variant` + `std::visit` 는 너무 장황하고, `enum class` + `switch` 는 데이터를 못 담는데?"
> → Rust `enum`은 **각 variant가 데이터를 가질 수 있는 tagged union** 입니다.

```cpp
// C++17
struct Circle { double r; };
struct Square { double side; };
using Shape = std::variant<Circle, Square>;

double area(const Shape& s) {
    return std::visit([](auto&& v) -> double {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, Circle>) return 3.14 * v.r * v.r;
        else return v.side * v.side;
    }, s);
}
```

```rust
// Rust
enum Shape {
    Circle { r: f64 },
    Square(f64),
    Point,
}

fn area(s: &Shape) -> f64 {
    match s {
        Shape::Circle { r } => 3.14 * r * r,
        Shape::Square(side) => side * side,
        Shape::Point => 0.0,
    }   // variant 하나라도 빠지면 컴파일 에러 (exhaustive check)
}
```

### match 패턴 활용

```rust
let n = 7;
let kind = match n {
    0 => "zero",
    1 | 2 | 3 => "small",
    4..=9 => "medium",
    x if x < 0 => "negative",   // guard
    _ => "large",               // default
};

// 튜플/구조체 분해 (structured binding 대체)
let (a, b) = (1, 2);
let Rect { w, h } = Rect::new(1.0, 2.0);   // 같은 모듈 안에서

// 하나의 경우만 관심 있을 때: if let
if let Shape::Circle { r } = shape {
    println!("radius {r}");
}

// let-else: 매칭 실패 시 조기 반환
let Some(x) = maybe_value else { return; };
```

### C 스타일 enum

```rust
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
enum Color { Red = 1, Green = 2, Blue = 4 }
let v = Color::Green as u8;  // 2
```

---

## 12. ⭐ `Option<T>` — null 대신

> **C++ 고민:** "포인터 반환 후 `nullptr` 체크를 깜빡하는 실수"
> → Rust에는 null이 없습니다. "값이 없을 수 있음"은 타입(`Option`)으로 표현되고, **확인하지 않으면 값을 꺼낼 수 없습니다.**

```rust
// enum Option<T> { Some(T), None }

fn find_user(id: u32) -> Option<String> {
    if id == 1 { Some("kim".to_string()) } else { None }
}

// 1) match
match find_user(1) {
    Some(name) => println!("{name}"),
    None => println!("not found"),
}

// 2) if let
if let Some(name) = find_user(1) { println!("{name}"); }

// 3) 기본값
let name = find_user(2).unwrap_or("guest".to_string());
let name = find_user(2).unwrap_or_default();       // ""
let name = find_user(2).unwrap_or_else(|| expensive_default());

// 4) 변환 체인 (std::optional::transform/and_then 과 동일)
let len = find_user(1).map(|s| s.len());           // Option<usize>
let upper = find_user(1).filter(|s| !s.is_empty()).map(|s| s.to_uppercase());

// 5) unwrap / expect: None이면 panic — 테스트나 "절대 None일 수 없음"이 확실할 때만
let name = find_user(1).expect("user 1 must exist");
```

참조를 반환할 땐 `Option<&T>`를 씁니다 (C++의 "못 찾으면 nullptr 반환하는 포인터" 대체).

```rust
fn find<'a>(v: &'a [String], key: &str) -> Option<&'a String> {
    v.iter().find(|s| s.as_str() == key)
}
```

---

## 13. ⭐ `Result<T, E>` — 예외 대신

> **C++ 고민:** "예외를 쓰자니 성능/흐름 추적이 어렵고, 에러코드를 쓰자니 체크를 잊는다."
> → `Result`는 반환 타입에 에러가 드러나고, 무시하면 **경고**가 뜹니다. `?` 연산자로 전파가 간결합니다.

```rust
// enum Result<T, E> { Ok(T), Err(E) }
use std::fs;
use std::num::ParseIntError;

fn parse_num(s: &str) -> Result<i32, ParseIntError> {
    s.trim().parse::<i32>()
}

fn double(s: &str) -> Result<i32, ParseIntError> {
    let n = parse_num(s)?;     // Err면 즉시 return Err(e) — 예외 전파처럼 동작
    Ok(n * 2)
}

match double("21") {
    Ok(v) => println!("{v}"),
    Err(e) => eprintln!("error: {e}"),
}

// 파일 읽기
fn read_config() -> Result<String, std::io::Error> {
    let text = fs::read_to_string("config.txt")?;
    Ok(text)
}

// main에서도 ? 사용 가능
fn main() -> Result<(), Box<dyn std::error::Error>> {
    let cfg = read_config()?;
    println!("{cfg}");
    Ok(())
}
```

### panic은 언제?
- `panic!`은 C++의 `std::abort()`/`assert` 실패에 가깝습니다: **버그(불변식 위반)** 에 사용.
- 복구 가능한 오류(파일 없음, 파싱 실패, 네트워크 오류)는 항상 `Result`.

```rust
assert!(x > 0, "x must be positive, got {x}");
assert_eq!(a, b);
debug_assert!(cheap_check());   // 디버그 빌드에서만 (NDEBUG assert)
unreachable!();
todo!();                         // 미구현 표시
```

---

## 14. 모듈과 가시성 (`#include` / namespace 대체)

헤더 파일이 없습니다. 파일 = 모듈이고, `pub`으로 노출 여부를 결정합니다.

```
src/
├── main.rs          // 크레이트 루트
├── network.rs       // mod network
└── storage/
    ├── mod.rs       // mod storage  (또는 src/storage.rs)
    └── disk.rs      // mod storage::disk
```

```rust
// main.rs
mod network;          // network.rs 를 모듈로 포함 (#include 와 비슷하지만 1회만 컴파일)
mod storage;

use storage::disk::DiskStore;           // using 선언
use std::collections::{HashMap, HashSet};

fn main() {
    network::connect("localhost");
    let _d = DiskStore::new();
}
```

```rust
// network.rs
pub fn connect(host: &str) { helper(); println!("{host}"); }
fn helper() {}        // private: 이 모듈(과 하위 모듈)에서만 사용 가능
```

```rust
// storage/mod.rs
pub mod disk;
```

| 가시성 | 의미 |
|--------|------|
| (없음) | 현재 모듈과 그 자식에서만 |
| `pub` | 어디서나 |
| `pub(crate)` | 같은 크레이트(라이브러리/바이너리) 안에서만 — C++의 "internal" 개념 |
| `pub(super)` | 부모 모듈까지 |

> `struct`를 `pub`으로 해도 **필드는 각각 `pub`을 붙여야** 공개됩니다.

---

## 15. 초급 종합 예제

간단한 단어 빈도 카운터:

```rust
use std::collections::HashMap;
use std::env;
use std::fs;

#[derive(Debug)]
struct WordStat {
    word: String,
    count: usize,
}

fn count_words(text: &str) -> Vec<WordStat> {
    let mut map: HashMap<String, usize> = HashMap::new();
    for w in text.split_whitespace() {
        let w = w.trim_matches(|c: char| !c.is_alphanumeric()).to_lowercase();
        if w.is_empty() { continue; }
        *map.entry(w).or_insert(0) += 1;
    }
    let mut stats: Vec<WordStat> = map
        .into_iter()
        .map(|(word, count)| WordStat { word, count })
        .collect();
    stats.sort_by(|a, b| b.count.cmp(&a.count).then(a.word.cmp(&b.word)));
    stats
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let path = env::args().nth(1).ok_or("usage: wc <file>")?;
    let text = fs::read_to_string(&path)?;
    for s in count_words(&text).iter().take(10) {
        println!("{:>10} {}", s.count, s.word);
    }
    Ok(())
}
```

---

## 초급 체크리스트

- [ ] `let` / `let mut` / `const` 차이를 안다
- [ ] move가 기본이고 `clone()`이 명시적 복사라는 걸 안다
- [ ] `&T` 여러 개 **또는** `&mut T` 하나 규칙을 이해한다
- [ ] 함수 인자로 `&str`, `&[T]`를 선호하는 이유를 안다
- [ ] `enum` + `match`로 variant를 처리할 수 있다
- [ ] `Option`, `Result`, `?`를 사용할 수 있다
- [ ] `mod`, `use`, `pub`으로 파일을 나눌 수 있다

다음: [02_Intermediate.md](02_Intermediate.md)
