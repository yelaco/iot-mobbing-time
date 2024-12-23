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
)

type GarbageBin struct {
	Id            string    `json:"id,omitempty"`
	Status        string    `json:"status,omitempty"`
	Location      []float64 `json:"location,omitempty"`
	FillLevel     int       `json:"fill_level,omitempty"`
	LastCollected string    `json:"last_collected,omitempty"`
}

func simulateGarbageBins(msgCh chan []byte, mapLastCollectedTime map[string]time.Time) {
	simData := []Data{
		{
			Id:        randomId(8),
			Location:  []float64{105.7858, 21.0267},
			FillLevel: 92,
			Collected: false,
		},
		{
			Id:        randomId(8),
			Location:  []float64{105.7881, 21.0267},
			FillLevel: 92,
			Collected: false,
		},
		{
			Id:        randomId(8),
			Location:  []float64{105.7907, 21.0236},
			FillLevel: 92,
			Collected: false,
		},
		{
			Id:        randomId(8),
			Location:  []float64{105.7923, 21.0275},
			FillLevel: 92,
			Collected: false,
		},
	}

	for _, data := range simData {
		mapLastCollectedTime[data.Id] = time.Now().Local().Add(-18 * time.Hour)
	}

	ctx, cancel := context.WithCancel(context.Background())

	for _, data := range simData {
		go func() {
			for {
				select {
				case <-ctx.Done():
					return
				default:
					<-time.After(time.Duration(5+rand.Intn(10)) * time.Second)

					data, err := json.Marshal(data)
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
