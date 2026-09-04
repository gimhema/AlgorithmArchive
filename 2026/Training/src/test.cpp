#include "test.h"
#include "my_memo.h"

#include <iostream>
#include <utility>
#include <cassert>

namespace
{
    struct Tracer
    {
        int id;
        explicit Tracer(int i) : id(i) { std::cout << "  Tracer(" << id << ") 생성\n"; }
        ~Tracer() { std::cout << "  Tracer(" << id << ") 소멸\n"; }
    };
}

void MY_LIB_TEST_PTR::TEST_MY_UNIQUE_PTR()
{
    std::cout << "=== TEST_MY_UNIQUE_PTR ===\n";
    {
        my_unique_ptr<Tracer> p(new Tracer(1));
        std::cout << "  스코프 안: p 사용 중\n";
    }
    std::cout << "  스코프 밖: 위에서 소멸 로그가 찍혔어야 정상\n";
}

void MY_LIB_TEST_PTR::TEST_MY_SHARED_PTR()
{
    std::cout << "=== TEST_MY_SHARED_PTR ===\n";

    std::cout << "-- 1. 기본 생성/소멸 --\n";
    {
        my_shared_ptr<Tracer> a(new Tracer(1));
        assert(a.get_cb()->strong == 1);
    }

    std::cout << "-- 2. 복사 생성: 카운트 공유 확인 --\n";
    {
        my_shared_ptr<Tracer> a(new Tracer(2));
        {
            my_shared_ptr<Tracer> b = a;
            assert(a.get_cb() == b.get_cb());
            assert(a.get_cb()->strong == 2);
            std::cout << "  b 생존 중 strong=" << a.get_cb()->strong << "\n";
        }
        std::cout << "  b 소멸 후 strong=" << a.get_cb()->strong << "\n";
        assert(a.get_cb()->strong == 1);
    }

    std::cout << "-- 3. 복사 대입 --\n";
    {
        my_shared_ptr<Tracer> a(new Tracer(3));
        my_shared_ptr<Tracer> b(new Tracer(4));
        b = a;
        assert(a.get_cb()->strong == 2);
        std::cout << "  대입 후 strong=" << a.get_cb()->strong << " (Tracer(4)는 위에서 소멸 로그 찍힘)\n";
    }

    std::cout << "-- 4. 이동 대입: 카운트 유지, 원본은 빈 상태 --\n";
    {
        my_shared_ptr<Tracer> a(new Tracer(5));
        my_shared_ptr<Tracer> b(new Tracer(6));
        auto* cbBefore = a.get_cb();
        b = std::move(a);
        assert(a.get() == nullptr);
        assert(b.get_cb() == cbBefore);
        assert(b.get_cb()->strong == 1);
        std::cout << "  이동 대입 후 strong=" << b.get_cb()->strong << "\n";
    }

    std::cout << "-- 5. reset(): 기존 리소스 놓고 새 control block --\n";
    {
        my_shared_ptr<Tracer> a(new Tracer(7));
        a.reset(new Tracer(8));
        assert(a.get_cb()->strong == 1);
    }

    std::cout << "-- 6. 순환 참조 (shared <-> shared): 의도적 leak 재현 --\n";
    {
        struct NodeB;
        struct NodeA
        {
            my_shared_ptr<NodeB> next;
            ~NodeA() { std::cout << "  ~NodeA (cycle)\n"; }
        };
        struct NodeB
        {
            my_shared_ptr<NodeA> next;
            ~NodeB() { std::cout << "  ~NodeB (cycle)\n"; }
        };

        auto a = my_shared_ptr<NodeA>(new NodeA());
        auto b = my_shared_ptr<NodeB>(new NodeB());
        a->next = b;
        b->next = a;

        std::cout << "  cycle 구성 후 a.strong=" << a.get_cb()->strong
                   << ", b.strong=" << b.get_cb()->strong << "\n";
    }
    std::cout << "  cycle 스코프 종료: 위에 ~NodeA/~NodeB 로그가 없으면 leak 재현 성공\n";
}

void MY_LIB_TEST_PTR::TEST_MY_WEAK_PTR()
{
    std::cout << "=== TEST_MY_WEAK_PTR ===\n";

    std::cout << "-- 1. 기본 expired/lock --\n";
    {
        my_shared_ptr<Tracer> a(new Tracer(10));
        my_weak_ptr<Tracer> w = a;
        assert(!w.expired());
        assert(a.get_cb()->weak == 1);

        auto locked = w.lock();
        assert(locked.get() != nullptr);
        assert(a.get_cb()->strong == 2);
        std::cout << "  lock() 후 strong=" << a.get_cb()->strong << "\n";
    }

    std::cout << "-- 2. shared_ptr가 먼저 죽고 weak_ptr만 남는 경우 --\n";
    {
        my_weak_ptr<Tracer> w;
        {
            my_shared_ptr<Tracer> a(new Tracer(11));
            w = a;
            assert(!w.expired());
        }
        std::cout << "  원본 shared_ptr 소멸 후 expired=" << w.expired() << "\n";
        assert(w.expired());
        auto locked = w.lock();
        assert(locked.get() == nullptr);
    }

    std::cout << "-- 3. shared_ptr 소멸 후에도 control block은 살아있음 --\n";
    {
        auto* raw = new my_shared_ptr<int>(new int(42));
        my_weak_ptr<int> w = *raw;
        auto* cb = raw->get_cb();
        delete raw;
        assert(cb->strong == 0);
        assert(cb->weak == 1);
        std::cout << "  shared 소멸 직후 cb->strong=" << cb->strong << ", cb->weak=" << cb->weak << "\n";
    }

    std::cout << "-- 4. 순환 참조를 weak_ptr로 끊기 --\n";
    {
        struct NodeB;
        struct NodeA
        {
            my_shared_ptr<NodeB> next;
            ~NodeA() { std::cout << "  ~NodeA (weak-break)\n"; }
        };
        struct NodeB
        {
            my_weak_ptr<NodeA> prev;
            ~NodeB() { std::cout << "  ~NodeB (weak-break)\n"; }
        };

        auto a = my_shared_ptr<NodeA>(new NodeA());
        auto b = my_shared_ptr<NodeB>(new NodeB());
        a->next = b;
        b->prev = a;

        std::cout << "  구성 후 a.strong=" << a.get_cb()->strong
                   << ", b.strong=" << b.get_cb()->strong << "\n";
    }
    std::cout << "  weak-break 스코프 종료: 위에 ~NodeA/~NodeB 둘 다 찍혔어야 정상 (leak 없음)\n";
}
