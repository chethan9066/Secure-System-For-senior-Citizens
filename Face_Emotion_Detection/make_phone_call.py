from twilio.rest import Client
import os

# Your Account Sid and Auth Token from twilio.com/console
# DANGER! This is insecure. See http://twil.io/secure
account_sid = os.environ['TWILIO_SID']
auth_token = os.environ['TWILIO_AUTH_TOKEN']
client = Client(account_sid, auth_token)

call = client.calls.create(
                        twiml='<Response><Say>please take care of Your parents they seems SAD today</Say></Response>',
                        to=os.environ['MY_NUMBER'],
                        from_='+19094559497'
                    )

print(call.sid)

