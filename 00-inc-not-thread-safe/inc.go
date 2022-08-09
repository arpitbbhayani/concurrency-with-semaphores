package main

import "sync"

var count int = 0

func incCount(wg *sync.WaitGroup, delta int) {
	for i := 0; i < delta; i++ {
		count += 1
	}
	wg.Done()
}

func parallelInc(numThreads int) (expectedCount int, observedCount int) {
	var wg sync.WaitGroup

	var incDelta int = 100

	expectedCount = numThreads * incDelta

	for i := 0; i < numThreads; i++ {
		wg.Add(1)
		go incCount(&wg, incDelta)
	}
	wg.Wait()

	observedCount = count
	return expectedCount, observedCount
}
