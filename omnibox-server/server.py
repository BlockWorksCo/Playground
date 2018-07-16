import os
from flask import Flask, request, redirect, url_for
from werkzeug import secure_filename

UPLOAD_FOLDER = '/tmp/'
ALLOWED_EXTENSIONS = set(['txt'])

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

@app.route("/", methods=['POST'])
def index():
    if request.method == 'POST':
        file = request.files['imageFile']
        if file:
            filename = secure_filename(file.filename)
            print('fileName : %s'%filename)
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
            return redirect(url_for('index'))
    return "200"

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5001, debug=True)


