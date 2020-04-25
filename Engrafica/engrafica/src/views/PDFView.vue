<template>
  <div class="pdfjs">
     <canvas id="the-canvas"></canvas>
  </div>
</template>

<script>


// https://mozilla.github.io/pdf.js/examples/

import pdfjsLib from 'pdfjs-dist/build/pdf';

export default
{
    //
    name: 'PDFView',

    props:
    {
        src:
        {
            type: String,
            required: true
        },

        filename:
        {
            type: String,
            required: true
        }
    },

    data ()
    {
        let a =
        {
            loading: false,
            timer: null,
            pdfData: atob(
              'JVBERi0xLjcKCjEgMCBvYmogICUgZW50cnkgcG9pbnQKPDwKICAvVHlwZSAvQ2F0YWxvZwog' +
              'IC9QYWdlcyAyIDAgUgo+PgplbmRvYmoKCjIgMCBvYmoKPDwKICAvVHlwZSAvUGFnZXMKICAv' +
              'TWVkaWFCb3ggWyAwIDAgMjAwIDIwMCBdCiAgL0NvdW50IDEKICAvS2lkcyBbIDMgMCBSIF0K' +
              'Pj4KZW5kb2JqCgozIDAgb2JqCjw8CiAgL1R5cGUgL1BhZ2UKICAvUGFyZW50IDIgMCBSCiAg' +
              'L1Jlc291cmNlcyA8PAogICAgL0ZvbnQgPDwKICAgICAgL0YxIDQgMCBSIAogICAgPj4KICA+' +
              'PgogIC9Db250ZW50cyA1IDAgUgo+PgplbmRvYmoKCjQgMCBvYmoKPDwKICAvVHlwZSAvRm9u' +
              'dAogIC9TdWJ0eXBlIC9UeXBlMQogIC9CYXNlRm9udCAvVGltZXMtUm9tYW4KPj4KZW5kb2Jq' +
              'Cgo1IDAgb2JqICAlIHBhZ2UgY29udGVudAo8PAogIC9MZW5ndGggNDQKPj4Kc3RyZWFtCkJU' +
              'CjcwIDUwIFRECi9GMSAxMiBUZgooSGVsbG8sIHdvcmxkISkgVGoKRVQKZW5kc3RyZWFtCmVu' +
              'ZG9iagoKeHJlZgowIDYKMDAwMDAwMDAwMCA2NTUzNSBmIAowMDAwMDAwMDEwIDAwMDAwIG4g' +
              'CjAwMDAwMDAwNzkgMDAwMDAgbiAKMDAwMDAwMDE3MyAwMDAwMCBuIAowMDAwMDAwMzAxIDAw' +
              'MDAwIG4gCjAwMDAwMDAzODAgMDAwMDAgbiAKdHJhaWxlcgo8PAogIC9TaXplIDYKICAvUm9v' +
              'dCAxIDAgUgo+PgpzdGFydHhyZWYKNDkyCiUlRU9G')

        };
        return a;
    },

    created ()
    {
    },

    beforeDestroy ()
    {
        document.removeEventListener('pagerendered', this.pageRendered);
        if (this.timer) clearInterval(this.timer);
    },

    mounted ()
    {
        pdfjsLib.GlobalWorkerOptions.workerSrc = './pdf.worker.js';
        //pdfjsLib.GlobalWorkerOptions.workerSrc = '//mozilla.github.io/pdf.js/build/pdf.worker.js';
        //var loadingTask = pdfjsLib.getDocument({data: this.pdfData});
        var loadingTask = pdfjsLib.getDocument('https://0.0.0.0:8081/MAX7400-MAX7407.pdf');

        loadingTask.promise.then(function(pdf) {
            console.log('PDF loaded'+pdf);

            // Fetch the first page
            var pageNumber = 1;
            pdf.getPage(pageNumber).then(function(page) {
                console.log('Page loaded');
                
                var scale = 1.5;
                var viewport = page.getViewport({scale: scale});

                // Prepare canvas using PDF page dimensions
                var canvas = document.getElementById('the-canvas');
                var context = canvas.getContext('2d');
                canvas.height = viewport.height;
                canvas.width = viewport.width;

                // Render PDF page into canvas context
                var renderContext = {
                    canvasContext: context,
                    viewport: viewport
                };
                var renderTask = page.render(renderContext);

                renderTask.promise.then(function () {
                    console.log('Page rendered');
                });
            });

        }, function (reason) {
            // PDF loading error
            console.error(reason);
        });
    },

    watch:
    {
        src: 'loadFile'
    },

    methods:
    {
        loadFile ()
        {
            this.loading = true;
            window.PDFJS.disableStream = true;
            window.PDFJS.disableRange = true;
            this.timer = setInterval(this.pollPDF, 100);
            window.PDFJS.workerSrc = './pdf.worker.js';
            if (window.PDFView.pdfDocument) window.PDFView.pdfDocument.destroy();
            window.PDFJS.webViewerLoad(this.src);
            window.PDFView.setScale('page-width', true);
        },

        pageRendered (evt)
        {
            console.log(evt);
            this.loading = false;
            clearInterval(this.timer);
            this.timer = null;
        },

    }

}
</script>

<style>
</style>




