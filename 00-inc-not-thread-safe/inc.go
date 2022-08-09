package main

import "sync"

var count int = 0

func incCount(wg *sync.WaitGroup, delta int) {
	for i := 0; i < delta; i++ {
		count += 1
	}
	wg.Done()
}
