from twilio.rest import Client
import os

# Your Account Sid and Auth Token from twilio.com/console
# DANGER! This is insecure. See http://twil.io/secure
<<<<<<< HEAD
account_sid = os.environ['SID']
auth_token = os.environ['AUTH_TOKEN']
client = Client(account_sid, auth_token)

call = client.calls.create(
                        twiml='<Response><Say>Ahoy, World!</Say></Response>',
=======
account_sid = os.environ['TWILIO_SID']
auth_token = os.environ['TWILIO_AUTH_TOKEN']
client = Client(account_sid, auth_token)

call = client.calls.create(
                        twiml='<Response><Say>please take care of Your parents they seems SAD today</Say></Response>',
>>>>>>> 10f4d98fd3c4af48e94b47eddeec84e9d8e806d0
                        to=os.environ['MY_NUMBER'],
                        from_='+19094559497'
                    )

print(call.sid)

