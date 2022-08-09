package main

import "sync"

var count int = 0

var wg *sync.WaitGroup

func reset() {
	wg = &sync.WaitGroup{}
	count = 0
}

func incCount(delta int) {
	for i := 0; i < delta; i++ {
		count += 1
	}
	wg.Done()
}

func parallelInc(numThreads int) (expectedCount int, observedCount int) {
	reset()

	var incDelta int = 100

	expectedCount = numThreads * incDelta

	for i := 0; i < numThreads; i++ {
		wg.Add(1)
		go incCount(incDelta)
	}
	wg.Wait()

	observedCount = count
	return expectedCount, observedCount
}
