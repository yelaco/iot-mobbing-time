import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS


bucket = "final-project"
org = "iot-mobbing-time"
token = "U3DTpR0HplFpB-fu65TeyttHrEN9QB2IDp244pOJjqEEim8MUXPo7oXvog1K9QYrBBPZQgrHrS80qVwWuF0bgA=="
url = "http://localhost:8086"

client = influxdb_client.InfluxDBClient(url=url, token=token, org=org)

# Query script
query_api = client.query_api()
query = 'from(bucket:"final-project")\
|> range(start: -24h)\
|> filter(fn:(r) => r._measurement == "trash-bin")'
result = query_api.query(org=org, query=query)
results = []
for table in result:
    for record in table.records:
        results.append((record.get_field(), record.get_value()))

print(results)
