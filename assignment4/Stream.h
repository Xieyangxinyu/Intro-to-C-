#pragma once

#include "DelayStreamImpl.h"
#include "EmptyStreamImpl.h"
#include "OnceStreamImpl.h"
#include "ChainStreamImpl.h"
#include "StreamImpl.h"

#include <memory>
#include <queue>
namespace stream
{

template <typename T>
class Stream {
private:
	std::shared_ptr<StreamImpl<T>> impl;
    Stream(const std::shared_ptr<StreamImpl<T>>& i): impl(i) {}
    
    // Make those functions friend of Stream<T>.
    // They are't static members of Stream<T> since we want to avoid the users having to explicitly specify T
    template <typename U> friend Stream<U> empty();
    template <typename U> friend Stream<U> once(U);
    template <typename U> friend Stream<U> chain(Stream<U>, Stream<U>);
    template <typename F> friend decltype(std::declval<F>()()) delay(F&&);
    template <typename U> friend Stream<U> take(const Stream<T>&, size_t);
    
    
public:
	using value_type = T;
    
	Optional<T> next() {
		return impl->next();
	}
};

template <typename T>
Stream<T> empty() {
	return Stream<T>(std::make_shared<EmptyStreamImpl<T>>());
}

template <typename F>
decltype(std::declval<F>()()) delay(F&& func) {
	using V = decltype(std::declval<F>()());
	using T = typename V::value_type;
	return V(std::make_shared<DelayStreamImpl<T, F>>(std::forward<F>(func)));
}
    
// You need to implement all the funtions listed below. Read the handout for more information
// Here is a checklist: 
// - once()
// - chain()
// - take()
// - filter()
// - map()
// - prime()
// - hamming()
// - pi()

template <typename T>
Stream<T> once(T val) {
    return Stream<T>(std::make_shared<OnceStreamImpl<T>>(val));
}

template <typename T>
Stream<T> chain(const Stream<T> first, const Stream<T> second) {
    return Stream<T>(std::make_shared<ChainStreamImpl<T>>(first, second));
}

template <typename T>
Stream<T> counter(T a) {
    return chain(
        once(a),
        delay([a] () { return counter(a + 1); } )
    );
}
    
template <typename T>//should we consider if n is greater than the length of the stream?
Stream<T> take(Stream<T> stream, size_t n) {
    auto s = once(*(stream.next()));
    for(int i = 1; i < n; i ++)
        s = chain(s, once(*(stream.next())));
    return s;
}

template <typename T, typename F>
Stream<T> filter(Stream<T> stream, F f) {
    Optional<T> v = stream.next();
    if(bool(v)){
        T val = *v;
        if (f(val)){
            return chain(once(val), delay([stream, f] () {return filter(stream, f);}));
        }
        else return delay([stream, f] () {return filter(stream, f);});
    }else
        return empty<T>();
}

template <typename S, typename F>
Stream<decltype(std::declval<F>()(std::declval<S>()))> map(Stream<S> stream, F f) {
    Optional<S> v = stream.next();
    if(bool(v)){
        S val = *v;
        return chain(once(f(val)), delay([stream, f] () {return map(stream, f);}));
    }else
        return empty<S>();
}

// The prime() function returns a stream of prime numbers, in increasing order
// A prime number is a natural number greater than 1 that has no positive divisors other than 1 and itself
// The first 20 prime numbers are { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71 }
Stream<int> sieve(Stream<int> s) {
    int val = *s.next();
    auto f = [val] (int num) { return num % val != 0;};
    s = filter(s, f);
    return chain(once(val), delay([s] () {return sieve(s);}));
}
    
Stream<int> prime() {
	return sieve(counter(2));
}

// (THIS IS A HELPER FUNCTION FOR hamming())
// The mergeUnique function takes two streams s0 and s1 as parameters
// Both s0 and s1 are sequences of increasing numbers (potentially infinite)
// Return a new stream of increasing numbers that contains all numbers from s0 and s1, removing duplicates
// e.g. if s0 = { 4, 6, 8, 10 } and s1 = { 3, 6, 9, 12, 15 }, the returning stream should be { 3, 4, 6, 8, 9, 10, 12, 15 }

Stream<int> mergeUnique(Stream<int> s0, Stream<int> s1) {
    Optional<int> tf = s0.next(), ts = s1.next();
    if(!(bool)tf && !(bool) ts) return empty<int>();
    if(!(bool)tf) return chain(once(*ts), s1);
    if(!(bool)ts) return chain(once(*tf), s0);
    
    int first = *tf, second = *ts, minx;
    
    if(first < second){
        minx = first;
    }else{
        minx = second;
    }
    auto f = [minx] (int num) {return num != minx;};
    s1 = chain(once(second), s1);
    s0 = chain(once(first), s0);
    s0 = filter(s0, f);
    s1 = filter(s1, f);
    return chain(once(minx), delay([s0, s1] () {return mergeUnique(s0, s1);}));
}

// The hamming() function returns a stream of hamming numbers, in increasing order
// A hamming sequence enumerate, in ascending order with no repetitions, all positive integers with no prime factors other than 2, 3, or 5
// The first 20 hamming numbers are { 1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 16, 18, 20, 24, 25, 27, 30, 32, 36 }

Stream<int> isHamming(std::priority_queue<int, std::vector<int>, std::greater<int>> q){
    int v = q.top(); q.pop();
    while (q.size() && q.top() == v) q.pop();
    q.push(v*2); q.push(v*3); q.push(v*5);
    return chain(once(v), delay([q] () {return isHamming(q);}));
}

Stream<int> hamming() {
    std::priority_queue<int, std::vector<int>, std::greater<int> > q;
    q.push(1);
    return isHamming(q);
}

// (THIS IS A HELPER FUNCTION FOR pi())
// The partialSum function takes a streams s of doubles (potentially infinite) and returns a new stream representing the partial sum of the input stream
// e.g. if s = { 1, 2, 3, 4, 5 }, the returning stream should be { 1, 3, 6, 10, 15 }
Stream<double> partialSum(Stream<double> s, double sum) {
    sum += *s.next();
    return chain(once(sum), delay([s, sum] () {return partialSum(s, sum);}));
}

// The pi() function returns a stream of increasingly accurate approximations of pi.
// Use the expansion series for pi/4 for the approximation: 
// pi/4 = 1 - 1/3 + 1/5 - 1/7 + 1/9 - ...
Stream<double> pi() {
    auto f = [] (double num) {
        double ans;
        if(((int)num) & 1) ans = 1;
        else ans = -1;
        return ans * 4 * (1/(2 * num - 1));
        };
    Stream<double> s = map(counter(1.0), f);
    return partialSum(s, 0.0);
}

}
