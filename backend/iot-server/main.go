package main

import (
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/signal"
	"time"

	"github.com/joho/godotenv"
	"github.com/r3labs/sse/v2"
)

func main() {
	err := godotenv.Load()
	if err != nil {
		log.Fatal("Error loading .env file")
	}

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

	msgCh := make(chan []byte)
	go startMqttSub(msgCh)
	go simulateGarbageBins(server)

	influxCh := WriteToInfluxdb(5)
	defer close(influxCh)

	go func(server *sse.Server) {
		for {
			var msgData Data
			d := json.NewDecoder(bytes.NewReader(<-msgCh))
			err := d.Decode(&msgData)
			if err != nil {
				log.Println(err)
			}
			garbageBin := GarbageBin{
				Id:            msgData.Id,
				Status:        "OK",
				Location:      msgData.Location,
				FillLevel:     msgData.FillLevel,
				LastCollected: time.Now().Local().Format(time.DateTime),
			}

			// Save data to database
			influxCh <- garbageBin

			// Send data to frontend
			data, err := json.Marshal(garbageBin)
			if err != nil {
				log.Println(err)
			}
			server.Publish("messages", &sse.Event{
				Data: data,
			})
			fmt.Println("Pushed: ", string(data))
		}
	}(server)

	httpServer := &http.Server{Addr: ":8888", Handler: mux}
	go func() {
		log.Fatal(httpServer.ListenAndServe())
	}()

	// Setting up signal capturing
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt)

	// Waiting for SIGINT (kill -2)
	<-stop

	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer cancel()
	log.Fatal(httpServer.Shutdown(ctx))
}
