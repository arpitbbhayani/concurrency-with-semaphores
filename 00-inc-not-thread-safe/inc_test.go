package main

import (
	"sync"
	"testing"
)

func TestCorrectness(t *testing.T) {
	var wg sync.WaitGroup

	var numThreads int = 1000
	var incDelta int = 100

	for i := 0; i < numThreads; i++ {
		wg.Add(1)
		go incCount(&wg, incDelta)
	}

	wg.Wait()

	expectedCount := numThreads * incDelta
	if count != expectedCount {
		t.Errorf("count expected = %d but seen to be %d\n", expectedCount, count)
	}
}
