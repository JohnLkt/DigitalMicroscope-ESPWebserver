import requests

# url = 'http://esp.local/led'
url = 'http://esp.local/pos'
data = {
    "xpos": "50",
    "ypos": "50",
    "zpos": "50",
}

response = requests.post(url, json=data)

if response.ok:
    print('Response:', response.text)
else:
    print('Failed to send POST request')
