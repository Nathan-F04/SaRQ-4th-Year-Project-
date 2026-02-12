import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email import encoders

# create message object instance
msg = MIMEMultipart()

# set the sender and recipient email addresses, and the password
sender = ''
recipient = ''
password =""

# set the subject and body of the email
msg['Subject'] = 'Test Email with Attachment'
msg['From'] = sender
msg['To'] = recipient

body = 'This is a test email with attachment.'

# attach the body of the email to the message object
msg.attach(MIMEText(body, 'plain'))

# open the file in binary
filename = "example.jpg"
attachment = open("example.jpg", "rb")

# create a MIMEBase object and set its attributes
file = MIMEBase('application', 'octet-stream')
file.set_payload((attachment).read())
encoders.encode_base64(file)
file.add_header('Content-Disposition', "attachment; filename= %s" % filename)

# attach the MIMEBase object to the message object
msg.attach(file)

# create a SMTP session
server = smtplib.SMTP('smtp.gmail.com', 587)

# start TLS for security
server.starttls()

# authenticate with the email account
server.login(sender, password)

# send the email
server.sendmail(sender, recipient, msg.as_string())
print("Email sent")

# terminate the SMTP session
server.quit()