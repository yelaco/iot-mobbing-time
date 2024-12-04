<template>
  <div class="app-container">
    <Header />
    <div class="content-container">
      <div class="map-container">
        <div id="sidebar">
          Longitude: {{ location.lng.toFixed(4) }} | Latitude: {{ location.lat.toFixed(4) }} | Zoom:
          {{ location.zoom.toFixed(2) }} |
          <template v-if="location.bearing">
            Bearing: {{ location.bearing.toFixed(2) }} |
          </template>
          <template v-if="location.pitch"> Pitch: {{ location.pitch.toFixed(2) }} | </template>
          <button @click="location = { lng: 105.78225, lat: 21.02876, bearing: 0, pitch: 0, zoom: 15.69 }">
            Reset
          </button>
          | <button @click="$refs.mapComponent.reloadMap(bins)">Reload map</button> |
          <button @click="$refs.mapComponent.generateRoute()">Generate route</button>
        </div>
        e
        <Map v-model="location" ref="mapComponent" />
      </div>

      <div class="table-container">
        <GarbageBinTable :bins="bins" @update-bins="updateBins" @view-in-map="viewBinInMap" />
      </div>
    </div>
  </div>
</template>

<script>
import Header from './components/Header.vue'
import Map from './components/Map.vue'
import GarbageBinTable from './components/GarbageBinTable.vue'
import '@site/node_modules/mapbox-gl/dist/mapbox-gl.css'
import '@mapbox/mapbox-gl-directions/dist/mapbox-gl-directions.css'

export default {
  components: {
    Header,
    Map,
    GarbageBinTable,
  },

  data: () => ({
    location: {
      lng: 105.7825,
      lat: 21.0284,
      bearing: 0,
      pitch: 0,
      zoom: 15.5,
    },

    bins: [],
  }),

  methods: {
    updateBins(newBins) {
      this.bins = newBins
    },
    viewBinInMap(binLocation) {
      this.$refs.mapComponent.viewInMap(binLocation)
    }
  },
}
</script>

<style scoped>
/* Main container with full height */
.app-container {
  display: flex;
  flex-direction: column;
  height: 100%;
  /* Full viewport height */
  width: 100%;
  margin: 16px;
  /* Add margin around the whole app */
  align-items: center;
  justify-content: center;
}

/* Content container to hold map and table */
.content-container {
  flex: 1;
  /* Take all remaining height below the header */
  display: flex;
  width: 80%;
  flex-direction: column;
  gap: 16px;
  /* Add spacing between map and table */
  margin: 40px;
}

/* Map container with dynamic width and height */
.map-container {
  flex: 1;
  /* Take up equal space in the content container */
  width: 100%;
  /* Full width */
  height: 100%;
  /* Take half the content container height */
  background-color: #f0f0f0;
  /* Fallback background color */
  border: 1px solid #ddd;
  /* Optional: Add a border */
  border-radius: 8px;
  /* Rounded corners */
  overflow: hidden;
  /* Prevent content overflow */
}

/* Table container with dynamic width and height */
.table-container {
  flex: 1;
  /* Take up equal space */
  width: 100%;
  /* Full width */
  height: 100%;
  /* Take remaining space */
  overflow-y: auto;
  /* Scroll if the content exceeds the height */
  background-color: #ffffff;
  /* White background */
  border: 1px solid #ddd;
  /* Optional: Add a border */
  border-radius: 8px;
  /* Rounded corners */
  padding: 16px;
  /* Add padding inside the table container */
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  /* Optional shadow */
}

#sidebar {
  background-color: rgb(35 55 75 / 90%);
  color: #fff;
  padding: 6px 12px;
  font-family: monospace;
  z-index: 1;
  position: absolute;
  top: 0;
  left: 0;
  margin: 12px;
  border-radius: 4px;
}

#helper {
  color: #fff;
  padding: 6px 12px;
  font-family: monospace;
  z-index: 1;
  position: relative;
  top: 0;
  left: 0;
  margin: 12px;
  border-radius: 4px;
}
</style>
