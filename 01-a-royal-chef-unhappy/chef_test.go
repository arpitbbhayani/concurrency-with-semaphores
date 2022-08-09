package main

import (
	"os"
	"strconv"
	"testing"

	"github.com/sirupsen/logrus"
)

func TestIfKingAngry(t *testing.T) {
	isKingAngry := Play()
	if isKingAngry {
		t.Error("king is angry")
	}
}

func BenchmarkPlay(b *testing.B) {
	logrus.SetLevel(logrus.ErrorLevel)
	var countAngry int = 1
	for i := 0; i < b.N; i++ {
		isKingAngry := Play()
		if isKingAngry {
			b.Setenv("COUNT_ANGRY", strconv.Itoa(countAngry))
		}
	}
	b.Logf("Executed the scenario %d times and the king got angry %s times", b.N, os.Getenv("COUNT_ANGRY"))
}
