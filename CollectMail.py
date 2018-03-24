





import getpass, poplib
import email


allowed_mimetypes = ["application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"]



mimes = ["image/tif","image/tiff","images/x-tif","image/x-tiff",
           "application/tif","application/tiff","application/x-tif",
           "application/x-tiff"]

def WriteAttachment(msg):
    for part in msg.walk():
        if part.get_type() in mimes:
            name = part.get_filename()
            data = part.get_payload(decode=True)
            f = file(name,'wb')
            f.write(data)
            f.close()


user = 'hrdata@blockworks.co' 
Mailbox = poplib.POP3_SSL('mail.blockworks.co', '995') 
Mailbox.user(user) 
Mailbox.pass_('hrdata') 
numMessages = len(Mailbox.list()[1])
for i in range(numMessages):
    for msg in Mailbox.retr(i+1)[1]:
        print msg
        m = email.message_from_string('\r\n'.join(msg))
        WriteAttachment(m)

Mailbox.quit()





