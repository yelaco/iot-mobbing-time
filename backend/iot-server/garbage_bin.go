package main

import (
	"context"
	"encoding/json"
	"log"
	"math/rand"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/r3labs/sse/v2"
)

type GarbageBin struct {
	Id            string    `json:"id,omitempty"`
	Status        string    `json:"status,omitempty"`
	Location      []float64 `json:"location,omitempty"`
	FillLevel     int       `json:"fill_level,omitempty"`
	LastCollected string    `json:"last_collected,omitempty"`
}

func simulateGarbageBins(server *sse.Server, msgCh chan []byte) {
	garbageBins := []GarbageBin{
		{
			Id:            randomId(8),
			Status:        "OK",
			Location:      []float64{105.7858, 21.0267},
			FillLevel:     92,
			LastCollected: time.Now().Add(-18 * time.Hour).Format(time.DateTime),
		},
		{
			Id:            randomId(8),
			Status:        "OK",
			Location:      []float64{105.7881, 21.0267},
			FillLevel:     92,
			LastCollected: time.Now().Add(-18 * time.Hour).Format(time.DateTime),
		},
		{
			Id:            randomId(8),
			Status:        "OK",
			Location:      []float64{105.7907, 21.0236},
			FillLevel:     92,
			LastCollected: time.Now().Add(-18 * time.Hour).Format(time.DateTime),
		},
		{
			Id:            randomId(8),
			Status:        "OK",
			Location:      []float64{105.7923, 21.0275},
			FillLevel:     92,
			LastCollected: time.Now().Add(-18 * time.Hour).Format(time.DateTime),
		},
	}

	ctx, cancel := context.WithCancel(context.Background())

	for _, garbageBin := range garbageBins {
		go func() {
			for {
				select {
				case <-ctx.Done():
					return
				default:
					<-time.After(time.Duration(5+rand.Intn(10)) * time.Second)

					data, err := json.Marshal(garbageBin)
					if err != nil {
						log.Println(err)
					}

					msgCh <- data
				}
			}
		}()
	}

	sigChan := make(chan os.Signal, 1)
	signal.Notify(sigChan, os.Interrupt, syscall.SIGTERM)
	<-sigChan

	cancel()
}
