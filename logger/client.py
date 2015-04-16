import requests
import pycurl
import json


# Spark Core "pixel" values - please don't hack my core !
# Remember to change token often...
device = "53ff6d066667574847342567"
token = "4a76d0c0d1d2079fdf848c0d21fad32b656e3cab"






# Get status of a particular device
def get_status(device, token) :
	r = requests.post("https://api.spark.io/v1/devices", data={"access_token" : token, "id" : device})
	print r.text
	return r



# Run a core-exposed command on a particular device
def run_command(device, token, command) :
	r = requests.post("https://api.spark.io/v1/devices/{}/{}".format(device, command), data={"access_token" : token})
	print json.loads(r.text)["result"]
	return r



# Callback function for live-streaming events
def event_callback(data) :
	print data



# Listen for streaming events
def listen(device, token, event) :
	c = pycurl.Curl()
	c.setopt(pycurl.URL, "https://api.spark.io/v1/devices/events".format(device, command))
	c.setopt(pycurl.CAINFO, certifi.where())
	c.setopt(c.HTTPHEADER, ["Authorization: Bearer {}".format(token)])
	c.setopt(pycurl.WRITEFUNCTION, event_callback)
	c.perform()