package main

import (
	"context"
	"log"
	"os"
	"time"

	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
	"github.com/influxdata/influxdb-client-go/v2/api/write"
)

func WriteToInfluxdb(numBins int) chan<- GarbageBin {
	token := os.Getenv("INFLUXDB_TOKEN")
	url := "http://localhost:8086"
	client := influxdb2.NewClient(url, token)
	org := "iot-mobbing-time"
	bucket := "final-project"
	writeAPI := client.WriteAPIBlocking(org, bucket)

	influxCh := make(chan GarbageBin, numBins)

	go func() {
		defer close(influxCh)
		for data := range influxCh {
			log.Println(data)
			tags := map[string]string{
				"id": data.Id,
			}
			fields := map[string]interface{}{
				"status":         data.Status,
				"location":       data.Location,
				"fill_level":     data.FillLevel,
				"last_collected": data.LastCollected,
			}
			point := write.NewPoint("trash-bin", tags, fields, time.Now())
			if err := writeAPI.WritePoint(context.Background(), point); err != nil {
				log.Println(err)
			}
			log.Println("Data saved to influxdb")
		}
	}()

	return influxCh
}
