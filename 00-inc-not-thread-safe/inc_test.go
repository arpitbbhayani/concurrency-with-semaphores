package main

import (
	"fmt"
	"os"
	"strconv"
	"testing"
)

var scenarios = []struct {
	numThread int
}{
	{numThread: 1},
	{numThread: 10},
	{numThread: 50},
	{numThread: 100},
	{numThread: 500},
	{numThread: 1000},
}

func Test(t *testing.T) {
	reset()
	expectedCount, observedCount := parallelInc(1000)
	if expectedCount != observedCount {
		t.Errorf("observed count %d did not match the expected count %d", observedCount, expectedCount)
	}
}

func BenchmarkInc(b *testing.B) {
	for _, v := range scenarios {
		b.Run(fmt.Sprintf("numThread-%d", v.numThread), func(b *testing.B) {
			countMatched := 0
			for i := 0; i < b.N; i++ {
				reset()
				expectedCount, observedCount := parallelInc(v.numThread)
				if expectedCount == observedCount {
					countMatched++
					b.Setenv("MATCHED_COUNT", strconv.Itoa(countMatched))
				}
			}
			b.Logf("expected count matched the observed count %s out of %d times when number of threads were %d", os.Getenv("MATCHED_COUNT"), b.N, v.numThread)
		})
	}
}
