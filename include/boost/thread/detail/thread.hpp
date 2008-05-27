#ifndef BOOST_THREAD_THREAD_COMMON_HPP
#define BOOST_THREAD_THREAD_COMMON_HPP
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007-8 Anthony Williams
 
#include <boost/thread/exceptions.hpp>
#include <ostream>
#include <boost/thread/detail/move.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/detail/thread_heap_alloc.hpp>
#include <boost/utility.hpp>
#include <boost/assert.hpp>
#include <list>
#include <algorithm>
#include <boost/ref.hpp>
#include <boost/cstdint.hpp>
#include <boost/bind.hpp>
#include <stdlib.h>
#include <memory>

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4251)
#endif

namespace boost
{
    class BOOST_THREAD_DECL thread
    {
    private:
        thread(thread&);
        thread& operator=(thread&);

        void release_handle();

        template<typename F>
        struct thread_data:
            detail::thread_data_base
        {
            F f;

#ifdef BOOST_HAS_RVALUE_REFS
            thread_data(F&& f_):
                f(static_cast<F&&>(f_))
            {}
#else
            thread_data(F f_):
                f(f_)
            {}
            thread_data(detail::thread_move_t<F> f_):
                f(f_)
            {}
#endif            
            void run()
            {
                f();
            }
        private:
            void operator=(thread_data&);
            thread_data(thread_data&);
        };
        
        mutable boost::mutex thread_info_mutex;
        detail::thread_data_ptr thread_info;

        void start_thread();
        
        explicit thread(detail::thread_data_ptr data);

        detail::thread_data_ptr get_thread_info() const;

#ifdef BOOST_HAS_RVALUE_REFS
        template<typename F>
        static inline detail::thread_data_ptr make_thread_info(F&& f)
        {
            return detail::thread_data_ptr(detail::heap_new<thread_data<F> >(static_cast<F&&>(f)));
        }
#else
        template<typename F>
        static inline detail::thread_data_ptr make_thread_info(F f)
        {
            return detail::thread_data_ptr(detail::heap_new<thread_data<F> >(f));
        }
        template<typename F>
        static inline detail::thread_data_ptr make_thread_info(boost::detail::thread_move_t<F> f)
        {
            return detail::thread_data_ptr(detail::heap_new<thread_data<F> >(f));
        }
#endif
    public:
        thread();
        ~thread();

#ifdef BOOST_HAS_RVALUE_REFS
        template <class F>
        thread(F&& f):
            thread_info(make_thread_info(static_cast<F&&>(f)))
        {
            start_thread();
        }

        thread(thread&& other)
        {
            thread_info.swap(other.thread_info);
        }
        
        thread& operator=(thread&& other)
        {
            thread_info=other.thread_info;
            other.thread_info.reset();
            return *this;
        }

        thread&& move()
        {
            return static_cast<thread&&>(*this);
        }
        
#else
        template <class F>
        explicit thread(F f):
            thread_info(make_thread_info(f))
        {
            start_thread();
        }

        template <class F>
        thread(detail::thread_move_t<F> f):
            thread_info(make_thread_info(f))
        {
            start_thread();
        }

        thread(detail::thread_move_t<thread> x)
        {
            thread_info=x->thread_info;
            x->thread_info.reset();
        }
        
        thread& operator=(detail::thread_move_t<thread> x)
        {
            thread new_thread(x);
            swap(new_thread);
            return *this;
        }
        
        operator detail::thread_move_t<thread>()
        {
            return move();
        }
        
        detail::thread_move_t<thread> move()
        {
            detail::thread_move_t<thread> x(*this);
            return x;
        }

#endif

        template <class F,class A1>
        thread(F f,A1 a1):
            thread_info(make_thread_info(boost::bind(boost::type<void>(),f,a1)))
        {
            start_thread();
        }
        template <class F,class A1,class A2>
        thread(F f,A1 a1,A2 a2):
            thread_info(make_thread_info(boost::bind(boost::type<void>(),f,a1,a2)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3>
        thread(F f,A1 a1,A2 a2,A3 a3):
            thread_info(make_thread_info(boost::bind(boost::type<void>(),f,a1,a2,a3)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3,class A4>
        thread(F f,A1 a1,A2 a2,A3 a3,A4 a4):
            thread_info(make_thread_info(boost::bind(boost::type<void>(),f,a1,a2,a3,a4)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3,class A4,class A5>
        thread(F f,A1 a1,A2 a2,A3 a3,A4 a4,A5 a5):
            thread_info(make_thread_info(boost::bind(boost::type<void>(),f,a1,a2,a3,a4,a5)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3,class A4,class A5,class A6>
        thread(F f,A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6):
            thread_info(make_thread_info(boost::bind(boost::type<void>(),f,a1,a2,a3,a4,a5,a6)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3,class A4,class A5,class A6,class A7>
        thread(F f,A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7):
            thread_info(make_thread_info(boost::bind(boost::type<void>(),f,a1,a2,a3,a4,a5,a6,a7)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8>
        thread(F f,A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8):
            thread_info(make_thread_info(boost::bind(boost::type<void>(),f,a1,a2,a3,a4,a5,a6,a7,a8)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9>
        thread(F f,A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9):
            thread_info(make_thread_info(boost::bind(boost::type<void>(),f,a1,a2,a3,a4,a5,a6,a7,a8,a9)))
        {
            start_thread();
        }

        void swap(thread& x)
        {
            thread_info.swap(x.thread_info);
        }

        class id;
        id get_id() const;


        bool joinable() const;
        void join();
        bool timed_join(const system_time& wait_until);

        template<typename TimeDuration>
        inline bool timed_join(TimeDuration const& rel_time)
        {
            return timed_join(get_system_time()+rel_time);
        }
        void detach();

        static unsigned hardware_concurrency();

        typedef detail::thread_data_base::native_handle_type native_handle_type;
        native_handle_type native_handle();

        // backwards compatibility
        bool operator==(const thread& other) const;
        bool operator!=(const thread& other) const;

        static inline void yield()
        {
            this_thread::yield();
        }
        
        static inline void sleep(const system_time& xt)
        {
            this_thread::sleep(xt);
        }

        // extensions
        void interrupt();
        bool interruption_requested() const;
    };

#ifdef BOOST_HAS_RVALUE_REFS
    inline thread&& move(thread&& t)
    {
        return t;
    }
#else
    inline detail::thread_move_t<thread> move(detail::thread_move_t<thread> t)
    {
        return t;
    }
#endif

    template<typename F>
    struct thread::thread_data<boost::reference_wrapper<F> >:
        detail::thread_data_base
    {
        F& f;
        
        thread_data(boost::reference_wrapper<F> f_):
            f(f_)
        {}
        
        void run()
        {
            f();
        }
    };

    template<typename F>
    struct thread::thread_data<const boost::reference_wrapper<F> >:
        detail::thread_data_base
    {
        F& f;
        
        thread_data(const boost::reference_wrapper<F> f_):
            f(f_)
        {}
        
        void run()
        {
            f();
        }
    };
    

    namespace this_thread
    {
        class BOOST_THREAD_DECL disable_interruption
        {
            disable_interruption(const disable_interruption&);
            disable_interruption& operator=(const disable_interruption&);
            
            bool interruption_was_enabled;
            friend class restore_interruption;
        public:
            disable_interruption();
            ~disable_interruption();
        };

        class BOOST_THREAD_DECL restore_interruption
        {
            restore_interruption(const restore_interruption&);
            restore_interruption& operator=(const restore_interruption&);
        public:
            explicit restore_interruption(disable_interruption& d);
            ~restore_interruption();
        };

        thread::id BOOST_THREAD_DECL get_id();

        void BOOST_THREAD_DECL interruption_point();
        bool BOOST_THREAD_DECL interruption_enabled();
        bool BOOST_THREAD_DECL interruption_requested();

        inline void sleep(xtime const& abs_time)
        {
            sleep(system_time(abs_time));
        }
    }

    class thread::id
    {
    private:
        detail::thread_data_ptr thread_data;
            
        id(detail::thread_data_ptr thread_data_):
            thread_data(thread_data_)
        {}
        friend class thread;
        friend id this_thread::get_id();
    public:
        id():
            thread_data()
        {}
            
        bool operator==(const id& y) const
        {
            return thread_data==y.thread_data;
        }
        
        bool operator!=(const id& y) const
        {
            return thread_data!=y.thread_data;
        }
        
        bool operator<(const id& y) const
        {
            return thread_data<y.thread_data;
        }
        
        bool operator>(const id& y) const
        {
            return y.thread_data<thread_data;
        }
        
        bool operator<=(const id& y) const
        {
            return !(y.thread_data<thread_data);
        }
        
        bool operator>=(const id& y) const
        {
            return !(thread_data<y.thread_data);
        }

        template<class charT, class traits>
        friend std::basic_ostream<charT, traits>& 
        operator<<(std::basic_ostream<charT, traits>& os, const id& x)
        {
            if(x.thread_data)
            {
                return os<<x.thread_data;
            }
            else
            {
                return os<<"{Not-any-thread}";
            }
        }
    };

    inline bool thread::operator==(const thread& other) const
    {
        return get_id()==other.get_id();
    }
    
    inline bool thread::operator!=(const thread& other) const
    {
        return get_id()!=other.get_id();
    }
        
    namespace detail
    {
        struct thread_exit_function_base
        {
            virtual ~thread_exit_function_base()
            {}
            virtual void operator()() const=0;
        };
        
        template<typename F>
        struct thread_exit_function:
            thread_exit_function_base
        {
            F f;
            
            thread_exit_function(F f_):
                f(f_)
            {}
            
            void operator()() const
            {
                f();
            }
        };
        
        void add_thread_exit_function(thread_exit_function_base*);
    }
    
    namespace this_thread
    {
        template<typename F>
        void at_thread_exit(F f)
        {
            detail::thread_exit_function_base* const thread_exit_func=detail::heap_new<detail::thread_exit_function<F> >(f);
            detail::add_thread_exit_function(thread_exit_func);
        }
    }

    class thread_group:
        private noncopyable
    {
    public:
        ~thread_group()
        {
            for(std::list<thread*>::iterator it=threads.begin(),end=threads.end();
                it!=end;
                ++it)
            {
                delete *it;
            }
        }

        template<typename F>
        thread* create_thread(F threadfunc)
        {
            boost::lock_guard<mutex> guard(m);
            std::auto_ptr<thread> new_thread(new thread(threadfunc));
            threads.push_back(new_thread.get());
            return new_thread.release();
        }
        
        void add_thread(thread* thrd)
        {
            if(thrd)
            {
                boost::lock_guard<mutex> guard(m);
                threads.push_back(thrd);
            }
        }
            
        void remove_thread(thread* thrd)
        {
            boost::lock_guard<mutex> guard(m);
            std::list<thread*>::iterator const it=std::find(threads.begin(),threads.end(),thrd);
            if(it!=threads.end())
            {
                threads.erase(it);
            }
        }
        
        void join_all()
        {
            boost::lock_guard<mutex> guard(m);
            
            for(std::list<thread*>::iterator it=threads.begin(),end=threads.end();
                it!=end;
                ++it)
            {
                (*it)->join();
            }
        }
        
        void interrupt_all()
        {
            boost::lock_guard<mutex> guard(m);
            
            for(std::list<thread*>::iterator it=threads.begin(),end=threads.end();
                it!=end;
                ++it)
            {
                (*it)->interrupt();
            }
        }
        
        size_t size() const
        {
            boost::lock_guard<mutex> guard(m);
            return threads.size();
        }
        
    private:
        std::list<thread*> threads;
        mutable mutex m;
    };
}

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif