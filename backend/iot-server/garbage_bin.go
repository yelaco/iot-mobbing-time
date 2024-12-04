package main

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"math/rand"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/r3labs/sse/v2"
)

type GarbageBin struct {
	Id            string     `json:"id,omitempty"`
	Status        string     `json:"status,omitempty"`
	SensorData    SensorData `json:"sensor_data,omitempty"`
	LastCollected string     `json:"last_collected,omitempty"`
}

type SensorData struct {
	Location  []float64 `json:"location,omitempty"`
	FillLevel int       `json:"fill_level,omitempty"`
}

type Location struct {
	Lat float64 `json:"lat,omitempty"`
	Lng float64 `json:"lng,omitempty"`
}

func simulateGarbageBins(server *sse.Server) {
	garbageBins := []GarbageBin{
		{
			Id:     randomId(8),
			Status: "OK",
			SensorData: SensorData{
				Location:  []float64{105.7858, 21.0267},
				FillLevel: 92,
			},
			LastCollected: time.Now().Add(-18 * time.Hour).Format(time.DateTime),
		},
		{
			Id:     randomId(8),
			Status: "OK",
			SensorData: SensorData{
				Location:  []float64{105.7881, 21.0267},
				FillLevel: 92,
			},
			LastCollected: time.Now().Add(-18 * time.Hour).Format(time.DateTime),
		},
		{
			Id:     randomId(8),
			Status: "OK",
			SensorData: SensorData{
				Location:  []float64{105.7907, 21.0236},
				FillLevel: 92,
			},
			LastCollected: time.Now().Add(-18 * time.Hour).Format(time.DateTime),
		},
		{
			Id:     randomId(8),
			Status: "OK",
			SensorData: SensorData{
				Location:  []float64{105.7923, 21.0275},
				FillLevel: 92,
			},
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
					data, err := json.Marshal(garbageBin)
					if err != nil {
						log.Println(err)
					}

					server.Publish("messages", &sse.Event{
						Data: data,
					})

					fmt.Println("Pushed")
					<-time.After(time.Duration(5+rand.Intn(10)) * time.Second)
				}
			}
		}()
	}

	sigChan := make(chan os.Signal, 1)
	signal.Notify(sigChan, os.Interrupt, syscall.SIGTERM)
	<-sigChan

	cancel()
}
