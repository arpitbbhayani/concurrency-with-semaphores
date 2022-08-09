Concurrency with Semaphores
===

Concurrency is essential to scale systems, but writing "performant" concurrent code is tricky given the number of things that could go wrong like

- deadlock
- unnecessary starvation
- inconsistencies

How, how can we get better?

You might think, hey! Let me see that JAVA or Golang gives me out of the box. But trust me, that's not enough.

To be good with concurrency we do not need a particular language, instead we need to solve some super-cool problems

So, in this series, we go through some super-interesting and fun concurrency problems and build our core understanding of concurrency.

The entire series is language agnostic, and we can build our solution in literally any programming language that supports Multi-threading. We would not be using any advanced data structures or functions or utilities to solve the problem.

The only thing we would be using is Semaphores :)

The entire series is based on one book called "The Little Book of Semaphores" which I read in college and thought of covering it in depth in this series.

So, if you want to learn Concurrency in a very fun and interesting way, ho along :)

# Examples

- 00-inc-not-thread-safe
