<template>
  <table>
    <thead>
      <tr>
        <th>ID</th>
        <th>Status</th>
        <th>Fill Level</th>
        <th>Last Collected</th>
        <th>Location</th>
      </tr>
    </thead>
    <tbody>
      <tr v-for="bin in bins" :key="bin.id">
        <td>{{ bin.id }}</td>
        <td>{{ bin.status }}</td>
        <td>{{ bin.fillLevel }}%</td>
        <td>{{ bin.lastCollected }}</td>
        <td>
          {{ bin.location }}
          <button v-if="bin.location" style="margin: 10px">View in map</button>
        </td>
      </tr>
    </tbody>
  </table>
</template>

<script>
export default {
  props: {
    bins: {
      type: Array,
      required: true,
    },
  },

  mounted() {
    const sse = new EventSource('http://localhost:8080/events?stream=messages')
    sse.onmessage = this.updateTable
  },

  methods: {
    updateTable(event) {
      let binUpdate = JSON.parse(event.data)
      let binFound = false
      const updatedBins = this.bins.map((bin) => {
        if (bin.id === binUpdate.id) {
          binFound = true
          return {
            ...bin,
            status: binUpdate.status,
            fillLevel: binUpdate.sensor_data.fill_level,
            lastCollected: binUpdate.last_collected,
            location: Object.values(binUpdate.sensor_data.location),
          }
        }

        return bin
      })

      if (!binFound) {
        updatedBins.push({
          id: binUpdate.id,
          status: binUpdate.status,
          fillLevel: binUpdate.sensor_data.fill_level,
          lastCollected: binUpdate.last_collected,
        })
      }

      this.$emit('update-bins', updatedBins)
    },
  },
}
</script>

<style>
.table-container {
  overflow-x: auto;
  background-color: #f9f9f9;
  border: 1px solid #ddd;
  border-radius: 8px;
  padding: 10px;
}

table {
  width: 100%;
  border-collapse: collapse;
  font-family: Arial, sans-serif;
  font-size: 14px;
}

thead th {
  background-color: #4caf50;
  color: black;
  text-align: center;
  padding: 10px;
  border: 1px solid #ddd;
}

tbody td {
  padding: 10px;
  border: 1px solid #ddd;
  text-align: center;
  color: black;
}

tbody tr:nth-child(even) {
  background-color: #f2f2f2;
}

tbody tr:hover {
  background-color: #ddd;
}
</style>
