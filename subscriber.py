import paho.mqtt.client as mqtt
import datetime
import ssl

def on_subscribe(client, userdata, mid, reason_code_list, properties):
    # Since we subscribed only for a single channel, reason_code_list contains
    # a single entry
    if reason_code_list[0].is_failure:
        print(f"Broker rejected you subscription: {reason_code_list[0]}")
    else:
        print(f"Broker granted the following QoS: {reason_code_list[0].value}")

def on_unsubscribe(client, userdata, mid, reason_code_list, properties):
    if len(reason_code_list) == 0 or not reason_code_list[0].is_failure:
        print("unsubscribe succeeded (if SUBACK is received in MQTTv3 it success)")
    else:
        print(f"Broker replied with failure: {reason_code_list[0]}")
    client.disconnect()

def on_message(client, userdata, message):
    ct = datetime.datetime.now()
    print("current time:-", ct)
    print(message.topic + " " + str(message.payload))

def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code.is_failure:
        print(f"Failed to connect: {reason_code}. loop_forever() will retry connection")
    else:
        client.subscribe("project/fft/output", qos=1)

mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttc.on_connect = on_connect
mqttc.on_message = on_message
mqttc.on_subscribe = on_subscribe
mqttc.on_unsubscribe = on_unsubscribe

mqttc._connect_timeout = 200

mqttc.tls_set(
    ca_certs="fft/main/certs/mosquitto.org.crt",
    certfile="fft/main/certs/client.crt",
    keyfile="fft/main/certs/client.key",
    cert_reqs=ssl.CERT_REQUIRED,
    tls_version=ssl.PROTOCOL_TLS,
    ciphers=None
)

mqttc.tls_insecure_set(False)

mqttc.connect("test.mosquitto.org", 8883)
mqttc.loop_forever()