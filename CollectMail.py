





import email
import poplib

mimes = ["text/csv", 'application/vnd.garmin.tcx+xml']

def WriteAttachment(msg):
    for part in msg.walk():
        print(part.get_content_type())
        #print(part)
        if part.get_content_type() in mimes:
            name = part.get_filename()
            data = part.get_payload(decode=True)

            print(name);
            f = file(name,'wb')
            f.write(data)
            f.close()

ms = poplib.POP3_SSL('mail.blockworks.co')
ms.user('hrdata@blockworks.co')
ms.pass_('hrdata')

l = ms.list()
print(l)

msgcount = len(l[1])
for i in range(msgcount):
    response, msg_as_list, size = ms.retr(i+1)
    #print(msg_as_list)
    msg = email.message_from_string('\r\n'.join(msg_as_list))
    WriteAttachment(msg)





