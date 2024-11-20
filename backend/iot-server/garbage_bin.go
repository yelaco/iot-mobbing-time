package main

import (
	"encoding/json"
	"fmt"
	"log"
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

	for _, garbageBin := range garbageBins {
		data, err := json.Marshal(garbageBin)
		if err != nil {
			log.Println(err)
		}

		server.Publish("messages", &sse.Event{
			Data: data,
		})

		fmt.Println("Pushed")
	}
}
