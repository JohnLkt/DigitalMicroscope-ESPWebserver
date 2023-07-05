import requests

url = 'http://esp.local/led'
data = {
    "brightness": "50"
}

response = requests.post(url, json=data)

if response.ok:
    print('Response:', response.text)
else:
    print('Failed to send POST request')
