import requests
import json

r = requests.get('http://127.0.0.1:8888/v1/chain/get_info')

info = json.loads(r.text)
for x in info:
	print (x, "", info[x])
