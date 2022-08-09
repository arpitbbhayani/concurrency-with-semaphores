package main

import (
	"math/rand"
	"sync"
	"time"

	"github.com/sirupsen/logrus"
)

var isLunchPrepared bool = false
var isKingAngry bool = false

func reset() {
	isLunchPrepared = false
	isKingAngry = false
}

func act(activity string) {
	time.Sleep(time.Duration(rand.Int()%20) * time.Millisecond)
	logrus.Debugln(activity)
}

func King(wg *sync.WaitGroup) {
	act("king woke up")
	act("king addressed the people")
	act("king went to the diing room")
	if !isLunchPrepared {
		act("king angry")
		isKingAngry = true
	} else {
		act("king ate the lunch")
	}

	wg.Done()
}

func Chef(wg *sync.WaitGroup) {
	act("chef woke up")
	act("chef started preparing lunch")
	act("chef prepared the lunch")
	isLunchPrepared = true
	act("chef ate the lunch")

	wg.Done()
}

func Play() bool {
	reset()

	var wg sync.WaitGroup
	wg.Add(2)

	go King(&wg)
	go Chef(&wg)

	wg.Wait()

	return isKingAngry
}
