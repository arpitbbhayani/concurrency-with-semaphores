package main

import (
	"fmt"
	"os"
	"strconv"
	"testing"
)

var table = []struct {
	numThread int
}{
	{numThread: 1},
	{numThread: 10},
	{numThread: 50},
	{numThread: 100},
	{numThread: 500},
	{numThread: 1000},
}

func BenchmarkInc(b *testing.B) {
	for _, v := range table {
		b.Run(fmt.Sprintf("numThread-%d", v.numThread), func(b *testing.B) {
			countMatched := 0
			for i := 0; i < b.N; i++ {
				expectedCount, observedCount := parallelInc(v.numThread)
				if expectedCount == observedCount {
					countMatched++
					b.Setenv("MATCHED_COUNT", strconv.Itoa(countMatched))
				}
				count = 0
			}
			b.Logf("expected count matched the observed count %s out of %d times when number of threads were %d", os.Getenv("MATCHED_COUNT"), b.N, v.numThread)
		})
	}
}

func TestCorrectness(t *testing.T) {
	expectedCount, observedCount := parallelInc(1000)
	if observedCount != expectedCount {
		t.Errorf("count expected = %d but seen to be %d\n", expectedCount, observedCount)
	}
}
