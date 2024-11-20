package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"time"

	"github.com/r3labs/sse/v2"
)

func main() {
	server := sse.New()
	server.CreateStream("messages")

	mux := http.NewServeMux()
	mux.HandleFunc("/events", func(w http.ResponseWriter, r *http.Request) {
		go func() {
			<-r.Context().Done()
			println("The client is disconnected")
		}()

		w.Header().Set("Access-Control-Allow-Origin", "*")

		server.ServeHTTP(w, r)
	})

	garbageBin := GarbageBin{
		Id:     "Z6AVN19H",
		Status: "OK",
		SensorData: SensorData{
			Location: Location{
				Lat: 105.7825,
				Lng: 21.0284,
			},
			FillLevel: 40,
		},
		LastCollected: time.Now().Local().Format(time.DateTime),
	}

	simulateGarbageBins(server)

	go func(server *sse.Server) {
		for {
			data, err := json.Marshal(garbageBin)
			if err != nil {
				log.Println(err)
			}

			server.Publish("messages", &sse.Event{
				Data: data,
			})
			fmt.Println("Pushed")

			<-time.After(5 * time.Second)
		}
	}(server)

	http.ListenAndServe(":8080", mux)
}
