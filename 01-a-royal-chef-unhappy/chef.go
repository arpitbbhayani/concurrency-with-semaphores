package main

import (
	"math/rand"
	"sync"
	"time"

	"github.com/sirupsen/logrus"
)

var wg *sync.WaitGroup

var isLunchPrepared bool = false
var isKingAngry bool = false

func reset() {
	isLunchPrepared = false
	isKingAngry = false

	wg = &sync.WaitGroup{}
}

func act(activity string) {
	time.Sleep(time.Duration(rand.Int()%20) * time.Millisecond)
	logrus.Info(activity)
}

func King() {
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

func Chef() {
	act("chef woke up")
	act("chef started preparing lunch")
	act("chef prepared the lunch")
	isLunchPrepared = true
	act("chef ate the lunch")

	wg.Done()
}

func Play() bool {
	reset()

	wg.Add(2)

	go King()
	go Chef()

	wg.Wait()

	return isKingAngry
}
