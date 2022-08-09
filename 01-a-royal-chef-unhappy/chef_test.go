package main

import (
	"os"
	"strconv"
	"testing"

	"github.com/sirupsen/logrus"
)

func Test(t *testing.T) {
	logrus.SetLevel(logrus.InfoLevel)
	reset()

	isKingAngry := Play()
	if isKingAngry {
		t.Error("king got angry!")
	}
}

func BenchmarkPlay(b *testing.B) {
	logrus.SetLevel(logrus.ErrorLevel)
	var countAngry int = 0
	for i := 0; i < b.N; i++ {
		reset()
		isKingAngry := Play()
		if isKingAngry {
			countAngry += 1
			b.Setenv("COUNT_ANGRY", strconv.Itoa(countAngry))
		}
	}
	b.Logf("Executed the scenario %d times and the king got angry %s times", b.N, os.Getenv("COUNT_ANGRY"))
}
