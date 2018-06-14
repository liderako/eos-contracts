import requests
import json
import sys

# HTTP API link
# https://developers.eos.io/eosio-nodeos/reference

def 	printJsonInfo(info):
	for x in info:
		print (x, info[x])

def 	getInfo():
	r = requests.get('http://127.0.0.1:8888/v1/chain/get_info')
	info = json.loads(r.text)
	return (info)

def 	getBlock(id):
	data = '{"block_num_or_id":' + str(id) + "}"
	r = requests.post('http://127.0.0.1:8888/v1/chain/get_block', data=data)
	info = json.loads(r.text)
	# printJsonInfo(info)
	return (info)

# getBlock(sys.argv[1])
