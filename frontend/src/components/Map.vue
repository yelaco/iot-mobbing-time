<template>
  <div ref="mapContainer" class="map-container"></div>
</template>

<script>
import mapboxgl from 'mapbox-gl'
import turf from 'turf'

mapboxgl.accessToken =
  'pk.eyJ1IjoieWVsYWNvIiwiYSI6ImNtM244N2lkMTE3cXIyaW9uZm9kaTZhNWwifQ.tPUrtVS4LLaQmXhEb11cLw'

export default {
  props: {
    modelValue: {
      type: Object,
      required: true,
    },
  },

  data: () => ({ map: null }),

  mounted() {
    const { lng, lat, zoom, bearing, pitch } = this.modelValue

    const truckLocation = [105.7913, 21.0297]
    let garbageBinLocationList = []
    let toCollectList = []

    const map = new mapboxgl.Map({
      container: this.$refs.mapContainer,
      style: 'mapbox://styles/mapbox/streets-v12',
      center: [lng, lat],
      bearing,
      pitch,
      zoom,
    })

    let garbageBins = turf.featureCollection(
      garbageBinLocationList.map((location) => turf.point(location)),
    )

    const nothing = turf.featureCollection([])

    map.on('load', async () => {
      const marker = document.createElement('div')
      marker.classList = 'truck'

      // Create a new marker
      new mapboxgl.Marker(marker).setLngLat(truckLocation).addTo(map)

      map.addLayer({
        id: 'garbage-bins',
        type: 'circle',
        source: {
          data: garbageBins,
          type: 'geojson',
        },
        paint: {
          'circle-radius': 20,
          'circle-color': 'white',
          'circle-stroke-color': '#3887be',
          'circle-stroke-width': 3,
        },
      })

      // Create a symbol layer on top of circle layer
      map.addLayer({
        id: 'garbage-bins-symbol',
        type: 'symbol',
        source: {
          data: garbageBins,
          type: 'geojson',
        },
        layout: {
          'icon-image': 'car',
          'icon-size': 1.5,
        },
        paint: {
          'text-color': '#3887be',
        },
      })

      // routes
      map.addSource('route', {
        type: 'geojson',
        data: nothing,
      })

      map.addLayer(
        {
          id: 'routeline-active',
          type: 'line',
          source: 'route',
          layout: {
            'line-join': 'round',
            'line-cap': 'round',
          },
          paint: {
            'line-color': '#3887be',
            'line-width': ['interpolate', ['linear'], ['zoom'], 12, 3, 22, 12],
          },
        },
        'waterway-label',
      )

      map.addLayer(
        {
          id: 'routearrows',
          type: 'symbol',
          source: 'route',
          layout: {
            'symbol-placement': 'line',
            'text-field': '▶',
            'text-size': ['interpolate', ['linear'], ['zoom'], 12, 24, 22, 60],
            'symbol-spacing': ['interpolate', ['linear'], ['zoom'], 12, 30, 22, 160],
            'text-keep-upright': false,
          },
          paint: {
            'text-color': '#3887be',
            'text-halo-color': 'hsl(55, 11%, 96%)',
            'text-halo-width': 3,
          },
        },
        'waterway-label',
      )
    })

    async function reloadMap(bins) {
      garbageBinLocationList = bins.map((bin) => bin.location)
      toCollectList = bins
        .filter((item) => {
          return item.fill_level >= 80
        })
        .map((bin) => bin.location)
      console.log(toCollectList)
      garbageBins = turf.featureCollection(
        garbageBinLocationList.map((location) => turf.point(location)),
      )

      this.map.removeLayer('garbage-bins')
      this.map.removeLayer('garbage-bins-symbol')
      this.map.removeLayer('routearrows')
      this.map.removeLayer('routeline-active')
      this.map.removeSource('route')
      this.map.removeSource('garbage-bins')
      this.map.removeSource('garbage-bins-symbol')
      this.map.addLayer({
        id: 'garbage-bins',
        type: 'circle',
        source: {
          data: garbageBins,
          type: 'geojson',
        },
        paint: {
          'circle-radius': 20,
          'circle-color': 'white',
          'circle-stroke-color': '#3887be',
          'circle-stroke-width': 3,
        },
      })

      // Create a symbol layer on top of circle layer
      this.map.addLayer({
        id: 'garbage-bins-symbol',
        type: 'symbol',
        source: {
          id: 'garbage-bins',
          data: garbageBins,
          type: 'geojson',
        },
        layout: {
          'icon-image': 'car',
          'icon-size': 1.5,
        },
        paint: {
          'text-color': '#3887be',
        },
      })
      this.map.addSource('route', {
        type: 'geojson',
        data: nothing,
      })
      this.map.addLayer(
        {
          id: 'routeline-active',
          type: 'line',
          source: 'route',
          layout: {
            'line-join': 'round',
            'line-cap': 'round',
          },
          paint: {
            'line-color': '#3887be',
            'line-width': ['interpolate', ['linear'], ['zoom'], 12, 3, 22, 12],
          },
        },
        'waterway-label',
      )
      this.map.addLayer(
        {
          id: 'routearrows',
          type: 'symbol',
          source: 'route',
          layout: {
            'symbol-placement': 'line',
            'text-field': '▶',
            'text-size': ['interpolate', ['linear'], ['zoom'], 12, 24, 22, 60],
            'symbol-spacing': ['interpolate', ['linear'], ['zoom'], 12, 30, 22, 160],
            'text-keep-upright': false,
          },
          paint: {
            'text-color': '#3887be',
            'text-halo-color': 'hsl(55, 11%, 96%)',
            'text-halo-width': 3,
          },
        },
        'waterway-label',
      )
    }

    async function generateRoute() {
      // Make a request to the Optimization API
      const query = await fetch(assembleQueryURL(), { method: 'GET' })
      const response = await query.json()

      // Create an alert for any requests that return an error
      if (response.code !== 'Ok') {
        const handleMessage =
          response.code === 'InvalidInput'
            ? 'Refresh to start a new route. For more information: https://docs.mapbox.com/api/navigation/optimization/#optimization-api-errors'
            : 'Try a different point.'
        alert(`${response.code} - ${response.message}\n\n${handleMessage}`)
        return
      }
      // Create a GeoJSON feature collection
      const routeGeoJSON = turf.featureCollection([turf.feature(response.trips[0].geometry)])
      // Update the `route` source by getting the route source
      // and setting the data equal to routeGeoJSON
      map.getSource('route').setData(routeGeoJSON)
    }

    // Here you'll specify all the parameters necessary for requesting a response from the Optimization API
    function assembleQueryURL() {
      // Store the location of the truck in a constant called coordinates
      console.log(toCollectList)
      const coordinates = [truckLocation, ...garbageBinLocationList]

      // Set the profile to `driving`
      // Coordinates will include the current location of the truck,
      return `https://api.mapbox.com/optimized-trips/v1/mapbox/driving/${coordinates.join(
        ';',
      )}?overview=full&steps=true&geometries=geojson&source=first&access_token=${
        mapboxgl.accessToken
      }`
    }

    const updateLocation = () => this.$emit('update:modelValue', this.getLocation())

    map.on('move', updateLocation)
    map.on('zoom', updateLocation)
    map.on('rotate', updateLocation)
    map.on('pitch', updateLocation)

    this.generateRoute = generateRoute
    this.reloadMap = reloadMap
    this.map = map
  },

  expose() {
    return {
      generateRoute: this.generateRoute,
      reloadMap: this.reloadMap,
    }
  },

  unmounted() {
    this.map.remove()
    this.map = null
  },

  watch: {
    modelValue(next) {
      const curr = this.getLocation()
      const map = this.map

      if (curr.lng != next.lng || curr.lat != next.lat)
        map.setCenter({ lng: next.lng, lat: next.lat })
      if (curr.pitch != next.pitch) map.setPitch(next.pitch)
      if (curr.bearing != next.bearing) map.setBearing(next.bearing)
      if (curr.zoom != next.zoom) map.setZoom(next.zoom)
    },
  },

  methods: {
    getLocation() {
      return {
        ...this.map.getCenter(),
        bearing: this.map.getBearing(),
        pitch: this.map.getPitch(),
        zoom: this.map.getZoom(),
      }
    },
    viewInMap(location) {
      this.map.setCenter({ lng: location[0], lat: location[1] })
    },
  },
}
</script>

<style>
.map-container {
  width: 100%;
  height: 100%;
  position: relative;
}

.truck {
  width: 20px;
  height: 20px;
  border: 2px solid #fff;
  border-radius: 50%;
  background: #3887be;
  pointer-events: none;
}
</style>
