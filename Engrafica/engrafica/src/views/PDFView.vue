<template>
  <div id="componentContainer" class="pdfjs">

          <div id="outerContainer">

            <div id="sidebarContainer">
              <div id="toolbarSidebar">
                <div class="splitToolbarButton toggled">
                  <button id="viewThumbnail" class="toolbarButton group toggled" title="Show Thumbnails" tabindex="2" data-l10n-id="thumbs">
                     <span data-l10n-id="thumbs_label">Thumbnails</span>
                  </button>
                  <button id="viewOutline" class="toolbarButton group" title="Show Document Outline" tabindex="3" data-l10n-id="outline">
                     <span data-l10n-id="outline_label">Document Outline</span>
                  </button>
                  <button id="viewAttachments" class="toolbarButton group" title="Show Attachments" tabindex="4" data-l10n-id="attachments">
                     <span data-l10n-id="attachments_label">Attachments</span>
                  </button>
                </div>
              </div>
              <div id="sidebarContent">
                <div id="thumbnailView">
                </div>
                <div id="outlineView" class="hidden">
                </div>
                <div id="attachmentsView" class="hidden">
                </div>
              </div>
            </div>  <!-- sidebarContainer -->

            <div id="mainContainer">
              <div class="findbar hidden doorHanger hiddenSmallView" id="findbar">
                <label for="findInput" class="toolbarLabel" data-l10n-id="find_label">Find:</label>
                <input id="findInput" class="toolbarField" tabindex="91">
                <div class="splitToolbarButton">
                  <button class="toolbarButton findPrevious" title="" id="findPrevious" tabindex="92" data-l10n-id="find_previous">
                    <span data-l10n-id="find_previous_label">Previous</span>
                  </button>
                  <div class="splitToolbarButtonSeparator"></div>
                  <button class="toolbarButton findNext" title="" id="findNext" tabindex="93" data-l10n-id="find_next">
                    <span data-l10n-id="find_next_label">Next</span>
                  </button>
                </div>
                <input type="checkbox" id="findHighlightAll" class="toolbarField">
                <label for="findHighlightAll" class="toolbarLabel" tabindex="94" data-l10n-id="find_highlight">Highlight all</label>
                <input type="checkbox" id="findMatchCase" class="toolbarField">
                <label for="findMatchCase" class="toolbarLabel" tabindex="95" data-l10n-id="find_match_case_label">Match case</label>
                <span id="findMsg" class="toolbarLabel"></span>
              </div>  <!-- findbar -->

              <div id="secondaryToolbar" class="secondaryToolbar hidden doorHangerRight">
                <div id="secondaryToolbarButtonContainer">
                  <button id="secondaryPresentationMode" class="secondaryToolbarButton presentationMode visibleLargeView" title="Switch to Presentation Mode" tabindex="51" data-l10n-id="presentation_mode">
                    <span data-l10n-id="presentation_mode_label">Presentation Mode</span>
                  </button>

                  <button id="secondaryOpenFile" class="secondaryToolbarButton openFile visibleLargeView" title="Open File" tabindex="52" data-l10n-id="open_file">
                    <span data-l10n-id="open_file_label">Open</span>
                  </button>

                  <button id="secondaryPrint" class="secondaryToolbarButton print visibleMediumView" title="Print" tabindex="53" data-l10n-id="print">
                    <span data-l10n-id="print_label">Print</span>
                  </button>

                  <button id="secondaryDownload" class="secondaryToolbarButton download visibleMediumView" title="Download" tabindex="54" data-l10n-id="download">
                    <span data-l10n-id="download_label">Download</span>
                  </button>

                  <a href="#" id="secondaryViewBookmark" class="secondaryToolbarButton bookmark visibleSmallView" title="Current view (copy or open in new window)" tabindex="55" data-l10n-id="bookmark">
                    <span data-l10n-id="bookmark_label">Current View</span>
                  </a>

                  <div class="horizontalToolbarSeparator visibleLargeView"></div>

                  <button id="firstPage" class="secondaryToolbarButton firstPage" title="Go to First Page" tabindex="56" data-l10n-id="first_page">
                    <span data-l10n-id="first_page_label">Go to First Page</span>
                  </button>
                  <button id="lastPage" class="secondaryToolbarButton lastPage" title="Go to Last Page" tabindex="57" data-l10n-id="last_page">
                    <span data-l10n-id="last_page_label">Go to Last Page</span>
                  </button>

                  <div class="horizontalToolbarSeparator"></div>

                  <button id="pageRotateCw" class="secondaryToolbarButton rotateCw" title="Rotate Clockwise" tabindex="58" data-l10n-id="page_rotate_cw">
                    <span data-l10n-id="page_rotate_cw_label">Rotate Clockwise</span>
                  </button>
                  <button id="pageRotateCcw" class="secondaryToolbarButton rotateCcw" title="Rotate Counterclockwise" tabindex="59" data-l10n-id="page_rotate_ccw">
                    <span data-l10n-id="page_rotate_ccw_label">Rotate Counterclockwise</span>
                  </button>

                  <div class="horizontalToolbarSeparator"></div>

                  <button id="toggleHandTool" class="secondaryToolbarButton handTool" title="Enable hand tool" tabindex="60" data-l10n-id="hand_tool_enable">
                    <span data-l10n-id="hand_tool_enable_label">Enable hand tool</span>
                  </button>

                  <div class="horizontalToolbarSeparator"></div>

                  <button id="documentProperties" class="secondaryToolbarButton documentProperties" title="Document Properties…" tabindex="61" data-l10n-id="document_properties">
                    <span data-l10n-id="document_properties_label">Document Properties…</span>
                  </button>
                </div>
              </div>  <!-- secondaryToolbar -->

              <div class="toolbar">
                <div id="toolbarContainer">
                  <div id="toolbarViewer">
                    <div id="toolbarViewerLeft">
                      <button id="sidebarToggle" class="toolbarButton" title="Toggle Sidebar" tabindex="11" data-l10n-id="toggle_sidebar">
                        <span data-l10n-id="toggle_sidebar_label">Toggle Sidebar</span>
                      </button>
                      <div class="toolbarButtonSpacer"></div>
                      <button id="viewFind" class="toolbarButton group hiddenSmallView" title="Find in Document" tabindex="12" data-l10n-id="findbar">
                         <span data-l10n-id="findbar_label">Find</span>
                      </button>
                      <div class="splitToolbarButton">
                        <button class="toolbarButton pageUp" title="Previous Page" id="previous" tabindex="13" data-l10n-id="previous">
                          <span data-l10n-id="previous_label">Previous</span>
                        </button>
                        <div class="splitToolbarButtonSeparator"></div>
                        <button class="toolbarButton pageDown" title="Next Page" id="next" tabindex="14" data-l10n-id="next">
                          <span data-l10n-id="next_label">Next</span>
                        </button>
                      </div>
                      <label id="pageNumberLabel" class="toolbarLabel" for="pageNumber" data-l10n-id="page_label">Page: </label>
                      <input type="number" id="pageNumber" class="toolbarField pageNumber" value="1" size="4" min="1" tabindex="15">
                      <span id="numPages" class="toolbarLabel"></span>
                    </div>
                    <div id="toolbarViewerRight">
                      <button id="presentationMode" class="toolbarButton presentationMode hiddenLargeView" title="Switch to Presentation Mode" tabindex="31" data-l10n-id="presentation_mode">
                        <span data-l10n-id="presentation_mode_label">Presentation Mode</span>
                      </button>

                      <button id="openFile" class="toolbarButton openFile hiddenLargeView" title="Open File" tabindex="32" data-l10n-id="open_file">
                        <span data-l10n-id="open_file_label">Open</span>
                      </button>

                      <button id="print" class="toolbarButton print hiddenMediumView" title="Print" tabindex="33" data-l10n-id="print">
                        <span data-l10n-id="print_label">Print</span>
                      </button>

                      <button id="download" class="toolbarButton download hiddenMediumView" title="Download" tabindex="34" data-l10n-id="download">
                        <span data-l10n-id="download_label">Download</span>
                      </button>
                      <a href="#" id="viewBookmark" class="toolbarButton bookmark hiddenSmallView" title="Current view (copy or open in new window)" tabindex="35" data-l10n-id="bookmark">
                        <span data-l10n-id="bookmark_label">Current View</span>
                      </a>

                      <div class="verticalToolbarSeparator hiddenSmallView"></div>

                      <button id="secondaryToolbarToggle" class="toolbarButton" title="Tools" tabindex="36" data-l10n-id="tools">
                        <span data-l10n-id="tools_label">Tools</span>
                      </button>
                    </div>
                    <div class="outerCenter">
                      <div class="innerCenter" id="toolbarViewerMiddle">
                        <div class="splitToolbarButton">
                          <button id="zoomOut" class="toolbarButton zoomOut" title="Zoom Out" tabindex="21" data-l10n-id="zoom_out">
                            <span data-l10n-id="zoom_out_label">Zoom Out</span>
                          </button>
                          <div class="splitToolbarButtonSeparator"></div>
                          <button id="zoomIn" class="toolbarButton zoomIn" title="Zoom In" tabindex="22" data-l10n-id="zoom_in">
                            <span data-l10n-id="zoom_in_label">Zoom In</span>
                           </button>
                        </div>
                        <span id="scaleSelectContainer" class="dropdownToolbarButton">
                           <select id="scaleSelect" title="Zoom" tabindex="23" data-l10n-id="zoom">
                            <option id="pageAutoOption" title="" value="auto" selected="selected" data-l10n-id="page_scale_auto">Automatic Zoom</option>
                            <option id="pageActualOption" title="" value="page-actual" data-l10n-id="page_scale_actual">Actual Size</option>
                            <option id="pageFitOption" title="" value="page-fit" data-l10n-id="page_scale_fit">Fit Page</option>
                            <option id="pageWidthOption" title="" value="page-width" data-l10n-id="page_scale_width">Full Width</option>
                            <option id="customScaleOption" title="" value="custom"></option>
                            <option title="" value="0.5" data-l10n-id="page_scale_percent" data-l10n-args='{ "scale": 50 }'>50%</option>
                            <option title="" value="0.75" data-l10n-id="page_scale_percent" data-l10n-args='{ "scale": 75 }'>75%</option>
                            <option title="" value="1" data-l10n-id="page_scale_percent" data-l10n-args='{ "scale": 100 }'>100%</option>
                            <option title="" value="1.25" data-l10n-id="page_scale_percent" data-l10n-args='{ "scale": 125 }'>125%</option>
                            <option title="" value="1.5" data-l10n-id="page_scale_percent" data-l10n-args='{ "scale": 150 }'>150%</option>
                            <option title="" value="2" data-l10n-id="page_scale_percent" data-l10n-args='{ "scale": 200 }'>200%</option>
                            <option title="" value="3" data-l10n-id="page_scale_percent" data-l10n-args='{ "scale": 300 }'>300%</option>
                            <option title="" value="4" data-l10n-id="page_scale_percent" data-l10n-args='{ "scale": 400 }'>400%</option>
                          </select>
                        </span>
                      </div>
                    </div>
                  </div>
                  <div id="loadingBar">
                    <div class="progress">
                      <div class="glimmer">
                      </div>
                    </div>
                  </div>
                </div>
              </div>

              <menu type="context" id="viewerContextMenu">
                <menuitem id="contextFirstPage" label="First Page"
                          data-l10n-id="first_page"></menuitem>
                <menuitem id="contextLastPage" label="Last Page"
                          data-l10n-id="last_page"></menuitem>
                <menuitem id="contextPageRotateCw" label="Rotate Clockwise"
                          data-l10n-id="page_rotate_cw"></menuitem>
                <menuitem id="contextPageRotateCcw" label="Rotate Counter-Clockwise"
                          data-l10n-id="page_rotate_ccw"></menuitem>
              </menu>

              <div id="viewerContainer" tabindex="0">
                <div id="viewer" class="pdfViewer"></div>
              </div>

              <div id="errorWrapper" hidden='true'>
                <div id="errorMessageLeft">
                  <span id="errorMessage"></span>
                  <button id="errorShowMore" data-l10n-id="error_more_info">
                    More Information
                  </button>
                  <button id="errorShowLess" data-l10n-id="error_less_info" hidden='true'>
                    Less Information
                  </button>
                </div>
                <div id="errorMessageRight">
                  <button id="errorClose" data-l10n-id="error_close">
                    Close
                  </button>
                </div>
                <div class="clearBoth"></div>
                <textarea id="errorMoreInfo" hidden='true' readonly="readonly"></textarea>
              </div>
            </div> <!-- mainContainer -->

            <div id="overlayContainer" class="hidden">
              <div id="passwordOverlay" class="container hidden">
                <div class="dialog">
                  <div class="row">
                    <p id="passwordText" data-l10n-id="password_label">Enter the password to open this PDF file:</p>
                  </div>
                  <div class="row">
                    <input type="password" id="password" class="toolbarField" />
                  </div>
                  <div class="buttonRow">
                    <button id="passwordCancel" class="overlayButton"><span data-l10n-id="password_cancel">Cancel</span></button>
                    <button id="passwordSubmit" class="overlayButton"><span data-l10n-id="password_ok">OK</span></button>
                  </div>
                </div>
              </div>
              <div id="documentPropertiesOverlay" class="container hidden">
                <div class="dialog">
                  <div class="row">
                    <span data-l10n-id="document_properties_file_name">File name:</span> <p id="fileNameField">-</p>
                  </div>
                  <div class="row">
                    <span data-l10n-id="document_properties_file_size">File size:</span> <p id="fileSizeField">-</p>
                  </div>
                  <div class="separator"></div>
                  <div class="row">
                    <span data-l10n-id="document_properties_title">Title:</span> <p id="titleField">-</p>
                  </div>
                  <div class="row">
                    <span data-l10n-id="document_properties_author">Author:</span> <p id="authorField">-</p>
                  </div>
                  <div class="row">
                    <span data-l10n-id="document_properties_subject">Subject:</span> <p id="subjectField">-</p>
                  </div>
                  <div class="row">
                    <span data-l10n-id="document_properties_keywords">Keywords:</span> <p id="keywordsField">-</p>
                  </div>
                  <div class="row">
                    <span data-l10n-id="document_properties_creation_date">Creation Date:</span> <p id="creationDateField">-</p>
                  </div>
                  <div class="row">
                    <span data-l10n-id="document_properties_modification_date">Modification Date:</span> <p id="modificationDateField">-</p>
                  </div>
                  <div class="row">
                    <span data-l10n-id="document_properties_creator">Creator:</span> <p id="creatorField">-</p>
                  </div>
                  <div class="separator"></div>
                  <div class="row">
                    <span data-l10n-id="document_properties_producer">PDF Producer:</span> <p id="producerField">-</p>
                  </div>
                  <div class="row">
                    <span data-l10n-id="document_properties_version">PDF Version:</span> <p id="versionField">-</p>
                  </div>
                  <div class="row">
                    <span data-l10n-id="document_properties_page_count">Page Count:</span> <p id="pageCountField">-</p>
                  </div>
                  <div class="buttonRow">
                    <button id="documentPropertiesClose" class="overlayButton"><span data-l10n-id="document_properties_close">Close</span></button>
                  </div>
                </div>
              </div>
            </div>  <!-- overlayContainer -->
          </div> <!-- outerContainer -->

          <div id="printContainer"></div>
          <div id="mozPrintCallback-shim" hidden>
            <div class="mozPrintCallback-dialog-box">
              <!-- TODO: Localise the following strings -->
              Preparing document for printing...
              <div class="progress-row">
                <progress value="0" max="100"></progress>
                <span class="relative-progress">0%</span>
              </div>
              <div class="progress-actions">
                <input type="button" value="Cancel" class="mozPrintCallback-cancel">
              </div>
            </div>
          </div>
  </div>
</template>

<script>


// https://mozilla.github.io/pdf.js/examples/
// https://github.com/mozilla/pdf.js/blob/master/examples/components/simpleviewer.js

//import * as viewer from 'pdfjs-dist/web/pdf_viewer.js';
//import pdfjsLib from 'pdfjs-dist/build/pdf';
import PDFJS from 'pdf.js-viewer/pdf.js'; 

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

    data: () => ({
    }),

    created ()
    {
    },

    beforeDestroy ()
    {
    },

    mounted ()
    {
        //PDFJS.webViewerLoad('https://0.0.0.0:8081/MAX7400-MAX7407.pdf');
        console.log(PDFJS);

        window.PDFJS.disableStream = true;
        window.PDFJS.disableRange = true;
        window.PDFJS.workerSrc = './pdf.worker.js';
        if (window.PDFView.pdfDocument) window.PDFView.pdfDocument.destroy();
        window.PDFJS.webViewerLoad('https://0.0.0.0:8081/MAX7400-MAX7407.pdf');
        window.PDFView.setScale('page-width', true);

        setInterval( () => {window.PDFView.forceRendering();} , 10000);
    },

    watch:
    {
        src: 'loadFile'
    },

    methods:
    {
        loadFile ()
        {
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
              @media print {
                #printContainer div {
                  page-break-after: always;
                  page-break-inside: avoid;
                }
              }
              #mozPrintCallback-shim {
                position: fixed;
                top: 0;
                left: 0;
                height: 100%;
                width: 100%;
                z-index: 9999999;

                display: block;
                text-align: center;
                background-color: rgba(0, 0, 0, 0.5);
              }
              #mozPrintCallback-shim[hidden] {
                display: none;
              }
              @media print {
                #mozPrintCallback-shim {
                  display: none;
                }
              }

              #mozPrintCallback-shim .mozPrintCallback-dialog-box {
                display: inline-block;
                margin: -50px auto 0;
                position: relative;
                top: 45%;
                left: 0;
                min-width: 220px;
                max-width: 400px;

                padding: 9px;

                border: 1px solid hsla(0, 0%, 0%, .5);
                border-radius: 2px;
                box-shadow: 0 1px 4px rgba(0, 0, 0, 0.3);

                background-color: #474747;

                color: hsl(0, 0%, 85%);
                font-size: 16px;
                line-height: 20px;
              }
              #mozPrintCallback-shim .progress-row {
                clear: both;
                padding: 1em 0;
              }
              #mozPrintCallback-shim progress {
                width: 100%;
              }
              #mozPrintCallback-shim .relative-progress {
                clear: both;
                float: right;
              }
              #mozPrintCallback-shim .progress-actions {
                clear: both;
              }




        #componentContainer {
            height: 100%;
        }



        .pdfjs .textLayer {
            position: absolute;
            left: 0;
            top: 0;
            right: 0;
            bottom: 0;
            overflow: hidden;
            opacity: .2
        }

        .pdfjs .textLayer>div {
            color: transparent;
            position: absolute;
            white-space: pre;
            cursor: text;
            -webkit-transform-origin: 0 0;
            -moz-transform-origin: 0 0;
            -o-transform-origin: 0 0;
            -ms-transform-origin: 0 0;
            transform-origin: 0 0
        }

        .pdfjs .textLayer .highlight {
            margin: -1px;
            padding: 1px;
            background-color: #b400aa;
            border-radius: 4px
        }

        .pdfjs .textLayer .highlight.begin {
            border-radius: 4px 0 0 4px
        }

        .pdfjs .textLayer .highlight.end {
            border-radius: 0 4px 4px 0
        }

        .pdfjs .textLayer .highlight.middle {
            border-radius: 0
        }

        .pdfjs .textLayer .highlight.selected {
            background-color: #006400
        }

        .pdfjs .textLayer ::selection {
            background: #00f
        }

        .pdfjs .textLayer ::-moz-selection {
            background: #00f
        }

        .pdfjs .pdfViewer .canvasWrapper {
            overflow: hidden
        }

        .pdfjs .pdfViewer .page {
            direction: ltr;
            width: 816px;
            height: 1056px;
            margin: 1px auto -8px;
            position: relative;
            overflow: visible;
            border: 9px solid transparent;
            background-clip: content-box;
            border-image: url(images/shadow.png) 9 9 repeat;
            background-color: #fff
        }

        body {
            height: 100%
        }

        .pdfjs .pdfViewer.removePageBorders .page {
            margin: 0 auto 10px;
            border: none
        }

        .pdfjs .pdfViewer .page canvas {
            margin: 0;
            display: block
        }

        .pdfjs .pdfViewer .page .loadingIcon {
            position: absolute;
            display: block;
            left: 0;
            top: 0;
            right: 0;
            bottom: 0;
            background: url(images/loading-icon.gif) center no-repeat
        }

        .pdfjs .pdfViewer .page .annotLink>a:hover {
            opacity: .2;
            background: #ff0;
            box-shadow: 0 2px 10px #ff0
        }

        .pdfjs .pdfPresentationMode:-webkit-full-screen .pdfViewer .page {
            margin-bottom: 100%;
            border: 0
        }

        .pdfjs .pdfPresentationMode:-moz-full-screen .pdfViewer .page {
            margin-bottom: 100%;
            border: 0
        }

        .pdfjs .pdfPresentationMode:-ms-fullscreen .pdfViewer .page {
            margin-bottom: 100%!important;
            border: 0
        }

        .pdfjs .pdfPresentationMode:fullscreen .pdfViewer .page {
            margin-bottom: 100%;
            border: 0
        }

        .pdfjs .pdfViewer .page .annotText>img {
            position: absolute;
            cursor: pointer
        }

        .pdfjs .pdfViewer .page .annotTextContentWrapper {
            position: absolute;
            width: 20em
        }

        .pdfjs .pdfViewer .page .annotTextContent {
            z-index: 200;
            float: left;
            max-width: 20em;
            background-color: #FF9;
            box-shadow: 0 2px 5px #333;
            border-radius: 2px;
            padding: .6em;
            cursor: pointer
        }

        .pdfjs .pdfViewer .page .annotTextContent>h1 {
            font-size: 1em;
            border-bottom: 1px solid #000;
            padding-bottom: .2em
        }

        .pdfjs .pdfViewer .page .annotTextContent>p {
            padding-top: .2em
        }

        .pdfjs .pdfViewer .page .annotLink>a {
            position: absolute;
            font-size: 1em;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%
        }

        .pdfjs .pdfViewer .page .annotLink>a
        /* -ms-a */

        {
            background: url("data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAA\ LAAAAAABAAEAAAIBRAA7") 0 0 repeat
        }

        .pdfjs * {
            padding: 0;
            margin: 0
        }

        html {
            width: 100%;
            height: 100%;
            font-size: 10px
        }

        .pdfjs input,
        .pdfjs button,
        .pdfjs select {
            font: message-box;
            outline: none
        }

        .pdfjs .hidden {
            display: none!important
        }

        .pdfjs [hidden] {
            display: none!important
        }

        .pdfjs #viewerContainer.pdfPresentationMode:-webkit-full-screen {
            top: 0;
            border-top: 2px solid transparent;
            background-color: #000;
            width: 100%;
            height: 100%;
            overflow: hidden;
            cursor: none;
            -webkit-user-select: none
        }

        .pdfjs #viewerContainer.pdfPresentationMode:-moz-full-screen {
            top: 0;
            border-top: 2px solid transparent;
            background-color: #000;
            width: 100%;
            height: 100%;
            overflow: hidden;
            cursor: none;
            -moz-user-select: none
        }

        .pdfjs #viewerContainer.pdfPresentationMode:-ms-fullscreen {
            top: 0!important;
            border-top: 2px solid transparent;
            width: 100%;
            height: 100%;
            overflow: hidden!important;
            cursor: none;
            -ms-user-select: none
        }

        .pdfjs #viewerContainer.pdfPresentationMode:-ms-fullscreen::-ms-backdrop {
            background-color: #000
        }

        .pdfjs #viewerContainer.pdfPresentationMode:fullscreen {
            top: 0;
            border-top: 2px solid transparent;
            background-color: #000;
            width: 100%;
            height: 100%;
            overflow: hidden;
            cursor: none;
            -webkit-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none
        }

        .pdfjs .pdfPresentationMode:-webkit-full-screen a:not(.internalLink) {
            display: none
        }

        .pdfjs .pdfPresentationMode:-moz-full-screen a:not(.internalLink) {
            display: none
        }

        .pdfjs .pdfPresentationMode:-ms-fullscreen a:not(.internalLink) {
            display: none!important
        }

        .pdfjs .pdfPresentationMode:fullscreen a:not(.internalLink) {
            display: none
        }

        .pdfjs .pdfPresentationMode:-webkit-full-screen .textLayer>div {
            cursor: none
        }

        .pdfjs .pdfPresentationMode:-moz-full-screen .textLayer>div {
            cursor: none
        }

        .pdfjs .pdfPresentationMode:-ms-fullscreen .textLayer>div {
            cursor: none
        }

        .pdfjs .pdfPresentationMode:fullscreen .textLayer>div {
            cursor: none
        }

        .pdfjs .pdfPresentationMode.pdfPresentationModeControls>*,
        .pdfjs .pdfPresentationMode.pdfPresentationModeControls .textLayer>div {
            cursor: default
        }

        .pdfjs .outerCenter {
            pointer-events: none;
            position: relative
        }

        html[dir='ltr'] .pdfjs .outerCenter {
            float: right;
            right: 50%
        }

        html[dir='rtl'] .pdfjs .outerCenter {
            float: left;
            left: 50%
        }

        .pdfjs .innerCenter {
            pointer-events: auto;
            position: relative
        }

        html[dir='ltr'] .pdfjs .innerCenter {
            float: right;
            right: -50%
        }

        html[dir='rtl'] .pdfjs .innerCenter {
            float: left;
            left: -50%
        }

        .pdfjs #outerContainer {
            width: 100%;
            height: 100%;
            position: relative;
            background-color: #404040;
            background-image: url(images/texture.png)
        }

        .pdfjs #sidebarContainer {
            position: absolute;
            top: 0;
            bottom: 0;
            width: 200px;
            visibility: hidden;
            -webkit-transition-duration: 200ms;
            -webkit-transition-timing-function: ease;
            transition-duration: 200ms;
            transition-timing-function: ease
        }

        html[dir='ltr'] .pdfjs #sidebarContainer {
            -webkit-transition-property: left;
            transition-property: left;
            left: -200px
        }

        html[dir='rtl'] .pdfjs #sidebarContainer {
            -webkit-transition-property: right;
            transition-property: right;
            right: -200px
        }

        .pdfjs #outerContainer.sidebarMoving>#sidebarContainer,
        .pdfjs #outerContainer.sidebarOpen>#sidebarContainer {
            visibility: visible
        }

        html[dir='ltr'] .pdfjs #outerContainer.sidebarOpen>#sidebarContainer {
            left: 0
        }

        html[dir='rtl'] .pdfjs #outerContainer.sidebarOpen>#sidebarContainer {
            right: 0
        }

        .pdfjs #mainContainer {
            position: absolute;
            top: 0;
            right: 0;
            bottom: 0;
            left: 0;
            min-width: 320px;
            -webkit-transition-duration: 200ms;
            -webkit-transition-timing-function: ease;
            transition-duration: 200ms;
            transition-timing-function: ease
        }

        html[dir='ltr'] .pdfjs #outerContainer.sidebarOpen>#mainContainer {
            -webkit-transition-property: left;
            transition-property: left;
            left: 200px
        }

        html[dir='rtl'] .pdfjs #outerContainer.sidebarOpen>#mainContainer {
            -webkit-transition-property: right;
            transition-property: right;
            right: 200px
        }

        .pdfjs #sidebarContent {
            top: 32px;
            bottom: 0;
            overflow: auto;
            -webkit-overflow-scrolling: touch;
            position: absolute;
            width: 200px;
            background-color: hsla(0, 0%, 0%, .1)
        }

        html[dir='ltr'] .pdfjs #sidebarContent {
            left: 0;
            box-shadow: inset -1px 0 0 hsla(0, 0%, 0%, .25)
        }

        html[dir='rtl'] .pdfjs #sidebarContent {
            right: 0;
            box-shadow: inset 1px 0 0 hsla(0, 0%, 0%, .25)
        }

        .pdfjs #viewerContainer {
            overflow: auto;
            -webkit-overflow-scrolling: touch;
            position: absolute;
            top: 32px;
            right: 0;
            bottom: 0;
            left: 0;
            outline: none
        }

        html[dir='ltr'] .pdfjs #viewerContainer {
            box-shadow: inset 1px 0 0 hsla(0, 0%, 100%, .05)
        }

        html[dir='rtl'] .pdfjs #viewerContainer {
            box-shadow: inset -1px 0 0 hsla(0, 0%, 100%, .05)
        }

        .pdfjs .toolbar {
            position: relative;
            left: 0;
            right: 0;
            z-index: 9999;
            cursor: default
        }

        .pdfjs #toolbarContainer {
            width: 100%
        }

        .pdfjs #toolbarSidebar {
            width: 200px;
            height: 32px;
            background-color: #424242;
            background-image: url(images/texture.png), linear-gradient(hsla(0, 0%, 30%, .99), hsla(0, 0%, 25%, .95))
        }

        html[dir='ltr'] .pdfjs #toolbarSidebar {
            box-shadow: inset -1px 0 0 rgba(0, 0, 0, 0.25), inset 0 -1px 0 hsla(0, 0%, 100%, .05), 0 1px 0 hsla(0, 0%, 0%, .15), 0 0 1px hsla(0, 0%, 0%, .1)
        }

        html[dir='rtl'] .pdfjs #toolbarSidebar {
            box-shadow: inset 1px 0 0 rgba(0, 0, 0, 0.25), inset 0 1px 0 hsla(0, 0%, 100%, .05), 0 1px 0 hsla(0, 0%, 0%, .15), 0 0 1px hsla(0, 0%, 0%, .1)
        }

        .pdfjs #toolbarContainer,
        .pdfjs .findbar,
        .pdfjs .secondaryToolbar {
            position: relative;
            height: 32px;
            background-color: #474747;
            background-image: url(images/texture.png), linear-gradient(hsla(0, 0%, 32%, .99), hsla(0, 0%, 27%, .95))
        }

        html[dir='ltr'] .pdfjs #toolbarContainer,
        .pdfjs .findbar,
        .pdfjs .secondaryToolbar {
            box-shadow: inset 1px 0 0 hsla(0, 0%, 100%, .08), inset 0 1px 1px hsla(0, 0%, 0%, .15), inset 0 -1px 0 hsla(0, 0%, 100%, .05), 0 1px 0 hsla(0, 0%, 0%, .15), 0 1px 1px hsla(0, 0%, 0%, .1)
        }

        html[dir='rtl'] .pdfjs #toolbarContainer,
        .pdfjs .findbar,
        .pdfjs .secondaryToolbar {
            box-shadow: inset -1px 0 0 hsla(0, 0%, 100%, .08), inset 0 1px 1px hsla(0, 0%, 0%, .15), inset 0 -1px 0 hsla(0, 0%, 100%, .05), 0 1px 0 hsla(0, 0%, 0%, .15), 0 1px 1px hsla(0, 0%, 0%, .1)
        }

        .pdfjs #toolbarViewer {
            height: 32px
        }

        .pdfjs #loadingBar {
            position: relative;
            width: 100%;
            height: 4px;
            background-color: #333;
            border-bottom: 1px solid #333
        }

        .pdfjs #loadingBar .progress {
            position: absolute;
            top: 0;
            left: 0;
            width: 0;
            height: 100%;
            background-color: #ddd;
            overflow: hidden;
            -webkit-transition: width 200ms;
            transition: width 200ms
        }

        @-webkit-keyframes progressIndeterminate {
            .pdfjs 0% {
                left: 0
            }
            .pdfjs 50% {
                left: 100%
            }
            .pdfjs 100% {
                left: 100%
            }
        }

        @keyframes progressIndeterminate {
            .pdfjs 0% {
                left: 0
            }
            .pdfjs 50% {
                left: 100%
            }
            .pdfjs 100% {
                left: 100%
            }
        }

        .pdfjs #loadingBar .progress.indeterminate {
            background-color: #999;
            -webkit-transition: none;
            transition: none
        }

        .pdfjs #loadingBar .indeterminate .glimmer {
            position: absolute;
            top: 0;
            left: 0;
            height: 100%;
            width: 50px;
            background-image: linear-gradient(to right, #999 0%, #fff 50%, #999 100%);
            background-size: 100% 100%;
            background-repeat: no-repeat;
            -webkit-animation: progressIndeterminate 2s linear infinite;
            animation: progressIndeterminate 2s linear infinite
        }

        .pdfjs .findbar,
        .pdfjs .secondaryToolbar {
            top: 32px;
            position: absolute;
            z-index: 10000;
            height: 32px;
            min-width: 16px;
            padding: 0 6px;
            margin: 4px 2px;
            color: hsl(0, 0%, 85%);
            font-size: 12px;
            line-height: 14px;
            text-align: left;
            cursor: default
        }

        html[dir='ltr'] .pdfjs .findbar {
            left: 68px
        }

        html[dir='rtl'] .pdfjs .findbar {
            right: 68px
        }

        .pdfjs .findbar label {
            -webkit-user-select: none;
            -moz-user-select: none
        }

        .pdfjs #findInput[data-status="pending"] {
            background-image: url(images/loading-small.png);
            background-repeat: no-repeat;
            background-position: right
        }

        html[dir='rtl'] .pdfjs #findInput[data-status="pending"] {
            background-position: left
        }

        .pdfjs .secondaryToolbar {
            padding: 6px;
            height: auto;
            z-index: 30000
        }

        html[dir='ltr'] .pdfjs .secondaryToolbar {
            right: 4px
        }

        html[dir='rtl'] .pdfjs .secondaryToolbar {
            left: 4px
        }

        .pdfjs #secondaryToolbarButtonContainer {
            max-width: 200px;
            max-height: 400px;
            overflow-y: auto;
            -webkit-overflow-scrolling: touch;
            margin-bottom: -4px
        }

        .pdfjs .doorHanger,
        .pdfjs .doorHangerRight {
            border: 1px solid hsla(0, 0%, 0%, .5);
            border-radius: 2px;
            box-shadow: 0 1px 4px rgba(0, 0, 0, 0.3)
        }

        .pdfjs .doorHanger:after,
        .pdfjs .doorHanger:before,
        .pdfjs .doorHangerRight:after,
        .pdfjs .doorHangerRight:before {
            bottom: 100%;
            border: solid transparent;
            content: " ";
            height: 0;
            width: 0;
            position: absolute;
            pointer-events: none
        }

        .pdfjs .doorHanger:after,
        .pdfjs .doorHangerRight:after {
            border-bottom-color: hsla(0, 0%, 32%, .99);
            border-width: 8px
        }

        .pdfjs .doorHanger:before,
        .pdfjs .doorHangerRight:before {
            border-bottom-color: hsla(0, 0%, 0%, .5);
            border-width: 9px
        }

        html[dir='ltr'] .pdfjs .doorHanger:after,
        html[dir='rtl'] .pdfjs .doorHangerRight:after {
            left: 13px;
            margin-left: -8px
        }

        html[dir='ltr'] .pdfjs .doorHanger:before,
        html[dir='rtl'] .pdfjs .doorHangerRight:before {
            left: 13px;
            margin-left: -9px
        }

        html[dir='rtl'] .pdfjs .doorHanger:after,
        html[dir='ltr'] .pdfjs .doorHangerRight:after {
            right: 13px;
            margin-right: -8px
        }

        html[dir='rtl'] .pdfjs .doorHanger:before,
        html[dir='ltr'] .pdfjs .doorHangerRight:before {
            right: 13px;
            margin-right: -9px
        }

        .pdfjs #findMsg {
            font-style: italic;
            color: #A6B7D0
        }

        .pdfjs #findInput.notFound {
            background-color: #f66
        }

        html[dir='ltr'] .pdfjs #toolbarViewerLeft {
            margin-left: -1px
        }

        html[dir='rtl'] .pdfjs #toolbarViewerRight {
            margin-right: -1px
        }

        html[dir='ltr'] .pdfjs #toolbarViewerLeft,
        html[dir='rtl'] .pdfjs #toolbarViewerRight {
            position: absolute;
            top: 0;
            left: 0
        }

        html[dir='ltr'] .pdfjs #toolbarViewerRight,
        html[dir='rtl'] .pdfjs #toolbarViewerLeft {
            position: absolute;
            top: 0;
            right: 0
        }

        html[dir='ltr'] .pdfjs #toolbarViewerLeft>*,
        html[dir='ltr'] .pdfjs #toolbarViewerMiddle>*,
        html[dir='ltr'] .pdfjs #toolbarViewerRight>*,
        html[dir='ltr'] .pdfjs .findbar>* {
            position: relative;
            float: left
        }

        html[dir='rtl'] .pdfjs #toolbarViewerLeft>*,
        html[dir='rtl'] .pdfjs #toolbarViewerMiddle>*,
        html[dir='rtl'] .pdfjs #toolbarViewerRight>*,
        html[dir='rtl'] .pdfjs .findbar>* {
            position: relative;
            float: right
        }

        html[dir='ltr'] .pdfjs .splitToolbarButton {
            margin: 3px 2px 4px 0;
            display: inline-block
        }

        html[dir='rtl'] .pdfjs .splitToolbarButton {
            margin: 3px 0 4px 2px;
            display: inline-block
        }

        html[dir='ltr'] .pdfjs .splitToolbarButton>.toolbarButton {
            border-radius: 0;
            float: left
        }

        html[dir='rtl'] .pdfjs .splitToolbarButton>.toolbarButton {
            border-radius: 0;
            float: right
        }

        .pdfjs .toolbarButton,
        .pdfjs .secondaryToolbarButton,
        .pdfjs .overlayButton {
            border: 0 none;
            background: none;
            width: 32px;
            height: 25px
        }

        .pdfjs .toolbarButton>span {
            display: inline-block;
            width: 0;
            height: 0;
            overflow: hidden
        }

        .pdfjs .toolbarButton[disabled],
        .pdfjs .secondaryToolbarButton[disabled],
        .pdfjs .overlayButton[disabled] {
            opacity: .5
        }

        .pdfjs .toolbarButton.group {
            margin-right: 0
        }

        .pdfjs .splitToolbarButton.toggled .toolbarButton {
            margin: 0
        }

        .pdfjs .splitToolbarButton:hover>.toolbarButton,
        .pdfjs .splitToolbarButton:focus>.toolbarButton,
        .pdfjs .splitToolbarButton.toggled>.toolbarButton,
        .pdfjs .toolbarButton.textButton {
            background-color: hsla(0, 0%, 0%, .12);
            background-image: linear-gradient(hsla(0, 0%, 100%, .05), hsla(0, 0%, 100%, 0));
            background-clip: padding-box;
            border: 1px solid hsla(0, 0%, 0%, .35);
            border-color: hsla(0, 0%, 0%, .32) hsla(0, 0%, 0%, .38) hsla(0, 0%, 0%, .42);
            box-shadow: 0 1px 0 hsla(0, 0%, 100%, .05) inset, 0 0 1px hsla(0, 0%, 100%, .15) inset, 0 1px 0 hsla(0, 0%, 100%, .05);
            -webkit-transition-property: background-color, border-color, box-shadow;
            -webkit-transition-duration: 150ms;
            -webkit-transition-timing-function: ease;
            transition-property: background-color, border-color, box-shadow;
            transition-duration: 150ms;
            transition-timing-function: ease
        }

        .pdfjs .splitToolbarButton>.toolbarButton:hover,
        .pdfjs .splitToolbarButton>.toolbarButton:focus,
        .pdfjs .dropdownToolbarButton:hover,
        .pdfjs .overlayButton:hover,
        .pdfjs .toolbarButton.textButton:hover,
        .pdfjs .toolbarButton.textButton:focus {
            background-color: hsla(0, 0%, 0%, .2);
            box-shadow: 0 1px 0 hsla(0, 0%, 100%, .05) inset, 0 0 1px hsla(0, 0%, 100%, .15) inset, 0 0 1px hsla(0, 0%, 0%, .05);
            z-index: 199
        }

        .pdfjs .splitToolbarButton>.toolbarButton {
            position: relative
        }

        html[dir='ltr'] .pdfjs .splitToolbarButton>.toolbarButton:first-child,
        html[dir='rtl'] .pdfjs .splitToolbarButton>.toolbarButton:last-child {
            position: relative;
            margin: 0;
            margin-right: -1px;
            border-top-left-radius: 2px;
            border-bottom-left-radius: 2px;
            border-right-color: transparent
        }

        html[dir='ltr'] .pdfjs .splitToolbarButton>.toolbarButton:last-child,
        html[dir='rtl'] .pdfjs .splitToolbarButton>.toolbarButton:first-child {
            position: relative;
            margin: 0;
            margin-left: -1px;
            border-top-right-radius: 2px;
            border-bottom-right-radius: 2px;
            border-left-color: transparent
        }

        .pdfjs .splitToolbarButtonSeparator {
            padding: 8px 0;
            width: 1px;
            background-color: hsla(0, 0%, 0%, .5);
            z-index: 99;
            box-shadow: 0 0 0 1px hsla(0, 0%, 100%, .08);
            display: inline-block;
            margin: 5px 0
        }

        html[dir='ltr'] .pdfjs .splitToolbarButtonSeparator {
            float: left
        }

        html[dir='rtl'] .pdfjs .splitToolbarButtonSeparator {
            float: right
        }

        .pdfjs .splitToolbarButton:hover>.splitToolbarButtonSeparator,
        .pdfjs .splitToolbarButton.toggled>.splitToolbarButtonSeparator {
            padding: 12px 0;
            margin: 1px 0;
            box-shadow: 0 0 0 1px hsla(0, 0%, 100%, .03);
            -webkit-transition-property: padding;
            -webkit-transition-duration: 10ms;
            -webkit-transition-timing-function: ease;
            transition-property: padding;
            transition-duration: 10ms;
            transition-timing-function: ease
        }

        .pdfjs .toolbarButton,
        .pdfjs .dropdownToolbarButton,
        .pdfjs .secondaryToolbarButton,
        .pdfjs .overlayButton {
            min-width: 16px;
            padding: 2px 6px 0;
            border: 1px solid transparent;
            border-radius: 2px;
            color: hsla(0, 0%, 100%, .8);
            font-size: 12px;
            line-height: 14px;
            -webkit-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            cursor: default;
            -webkit-transition-property: background-color, border-color, box-shadow;
            -webkit-transition-duration: 150ms;
            -webkit-transition-timing-function: ease;
            transition-property: background-color, border-color, box-shadow;
            transition-duration: 150ms;
            transition-timing-function: ease
        }

        html[dir='ltr'] .pdfjs .toolbarButton,
        html[dir='ltr'] .pdfjs .overlayButton,
        html[dir='ltr'] .pdfjs .dropdownToolbarButton {
            margin: 3px 2px 4px 0
        }

        html[dir='rtl'] .pdfjs .toolbarButton,
        html[dir='rtl'] .pdfjs .overlayButton,
        html[dir='rtl'] .pdfjs .dropdownToolbarButton {
            margin: 3px 0 4px 2px
        }

        .pdfjs .toolbarButton:hover,
        .pdfjs .toolbarButton:focus,
        .pdfjs .dropdownToolbarButton,
        .pdfjs .overlayButton,
        .pdfjs .secondaryToolbarButton:hover,
        .pdfjs .secondaryToolbarButton:focus {
            background-color: hsla(0, 0%, 0%, .12);
            background-image: linear-gradient(hsla(0, 0%, 100%, .05), hsla(0, 0%, 100%, 0));
            background-clip: padding-box;
            border: 1px solid hsla(0, 0%, 0%, .35);
            border-color: hsla(0, 0%, 0%, .32) hsla(0, 0%, 0%, .38) hsla(0, 0%, 0%, .42);
            box-shadow: 0 1px 0 hsla(0, 0%, 100%, .05) inset, 0 0 1px hsla(0, 0%, 100%, .15) inset, 0 1px 0 hsla(0, 0%, 100%, .05)
        }

        .pdfjs .toolbarButton:hover:active,
        .pdfjs .overlayButton:hover:active,
        .pdfjs .dropdownToolbarButton:hover:active,
        .pdfjs .secondaryToolbarButton:hover:active {
            background-color: hsla(0, 0%, 0%, .2);
            background-image: linear-gradient(hsla(0, 0%, 100%, .05), hsla(0, 0%, 100%, 0));
            border-color: hsla(0, 0%, 0%, .35) hsla(0, 0%, 0%, .4) hsla(0, 0%, 0%, .45);
            box-shadow: 0 1px 1px hsla(0, 0%, 0%, .1) inset, 0 0 1px hsla(0, 0%, 0%, .2) inset, 0 1px 0 hsla(0, 0%, 100%, .05);
            -webkit-transition-property: background-color, border-color, box-shadow;
            -webkit-transition-duration: 10ms;
            -webkit-transition-timing-function: linear;
            transition-property: background-color, border-color, box-shadow;
            transition-duration: 10ms;
            transition-timing-function: linear
        }

        .pdfjs .toolbarButton.toggled,
        .pdfjs .splitToolbarButton.toggled>.toolbarButton.toggled,
        .pdfjs .secondaryToolbarButton.toggled {
            background-color: hsla(0, 0%, 0%, .3);
            background-image: linear-gradient(hsla(0, 0%, 100%, .05), hsla(0, 0%, 100%, 0));
            border-color: hsla(0, 0%, 0%, .4) hsla(0, 0%, 0%, .45) hsla(0, 0%, 0%, .5);
            box-shadow: 0 1px 1px hsla(0, 0%, 0%, .1) inset, 0 0 1px hsla(0, 0%, 0%, .2) inset, 0 1px 0 hsla(0, 0%, 100%, .05);
            -webkit-transition-property: background-color, border-color, box-shadow;
            -webkit-transition-duration: 10ms;
            -webkit-transition-timing-function: linear;
            transition-property: background-color, border-color, box-shadow;
            transition-duration: 10ms;
            transition-timing-function: linear
        }

        .pdfjs .toolbarButton.toggled:hover:active,
        .pdfjs .splitToolbarButton.toggled>.toolbarButton.toggled:hover:active,
        .pdfjs .secondaryToolbarButton.toggled:hover:active {
            background-color: hsla(0, 0%, 0%, .4);
            border-color: hsla(0, 0%, 0%, .4) hsla(0, 0%, 0%, .5) hsla(0, 0%, 0%, .55);
            box-shadow: 0 1px 1px hsla(0, 0%, 0%, .2) inset, 0 0 1px hsla(0, 0%, 0%, .3) inset, 0 1px 0 hsla(0, 0%, 100%, .05)
        }

        .pdfjs .dropdownToolbarButton {
            width: 120px;
            max-width: 120px;
            padding: 0;
            overflow: hidden;
            background: url(images/toolbarButton-menuArrows.png) no-repeat
        }

        html[dir='ltr'] .pdfjs .dropdownToolbarButton {
            background-position: 95%
        }

        html[dir='rtl'] .pdfjs .dropdownToolbarButton {
            background-position: 5%
        }

        .pdfjs .dropdownToolbarButton>select {
            min-width: 140px;
            font-size: 12px;
            color: hsl(0, 0%, 95%);
            margin: 0;
            padding: 3px 2px 2px;
            border: none;
            background: rgba(0, 0, 0, 0)
        }

        .pdfjs .dropdownToolbarButton>select>option {
            background: hsl(0, 0%, 24%)
        }

        .pdfjs #customScaleOption {
            display: none
        }

        .pdfjs #pageWidthOption {
            border-bottom: 1px rgba(255, 255, 255, .5) solid
        }

        html[dir='ltr'] .pdfjs .splitToolbarButton:first-child,
        html[dir='ltr'] .pdfjs .toolbarButton:first-child,
        html[dir='rtl'] .pdfjs .splitToolbarButton:last-child,
        html[dir='rtl'] .pdfjs .toolbarButton:last-child {
            margin-left: 4px
        }

        html[dir='ltr'] .pdfjs .splitToolbarButton:last-child,
        html[dir='ltr'] .pdfjs .toolbarButton:last-child,
        html[dir='rtl'] .pdfjs .splitToolbarButton:first-child,
        html[dir='rtl'] .pdfjs .toolbarButton:first-child {
            margin-right: 4px
        }

        .pdfjs .toolbarButtonSpacer {
            width: 30px;
            display: inline-block;
            height: 1px
        }

        .pdfjs .toolbarButtonFlexibleSpacer {
            -webkit-box-flex: 1;
            -moz-box-flex: 1;
            min-width: 30px
        }

        html[dir='ltr'] .pdfjs #findPrevious {
            margin-left: 3px
        }

        html[dir='ltr'] .pdfjs #findNext {
            margin-right: 3px
        }

        html[dir='rtl'] .pdfjs #findPrevious {
            margin-right: 3px
        }

        html[dir='rtl'] .pdfjs #findNext {
            margin-left: 3px
        }

        .pdfjs .toolbarButton::before,
        .pdfjs .secondaryToolbarButton::before {
            position: absolute;
            display: inline-block;
            top: 4px;
            left: 7px
        }

        html[dir="ltr"] .pdfjs .secondaryToolbarButton::before {
            left: 4px
        }

        html[dir="rtl"] .pdfjs .secondaryToolbarButton::before {
            right: 4px
        }

        html[dir='ltr'] .pdfjs .toolbarButton#sidebarToggle::before {
            content: url(images/toolbarButton-sidebarToggle.png)
        }

        html[dir='rtl'] .pdfjs .toolbarButton#sidebarToggle::before {
            content: url(images/toolbarButton-sidebarToggle-rtl.png)
        }

        html[dir='ltr'] .pdfjs .toolbarButton#secondaryToolbarToggle::before {
            content: url(images/toolbarButton-secondaryToolbarToggle.png)
        }

        html[dir='rtl'] .pdfjs .toolbarButton#secondaryToolbarToggle::before {
            content: url(images/toolbarButton-secondaryToolbarToggle-rtl.png)
        }

        html[dir='ltr'] .pdfjs .toolbarButton.findPrevious::before {
            content: url(images/findbarButton-previous.png)
        }

        html[dir='rtl'] .pdfjs .toolbarButton.findPrevious::before {
            content: url(images/findbarButton-previous-rtl.png)
        }

        html[dir='ltr'] .pdfjs .toolbarButton.findNext::before {
            content: url(images/findbarButton-next.png)
        }

        html[dir='rtl'] .pdfjs .toolbarButton.findNext::before {
            content: url(images/findbarButton-next-rtl.png)
        }

        html[dir='ltr'] .pdfjs .toolbarButton.pageUp::before {
            content: url(images/toolbarButton-pageUp.png)
        }

        html[dir='rtl'] .pdfjs .toolbarButton.pageUp::before {
            content: url(images/toolbarButton-pageUp-rtl.png)
        }

        html[dir='ltr'] .pdfjs .toolbarButton.pageDown::before {
            content: url(images/toolbarButton-pageDown.png)
        }

        html[dir='rtl'] .pdfjs .toolbarButton.pageDown::before {
            content: url(images/toolbarButton-pageDown-rtl.png)
        }

        .pdfjs .toolbarButton.zoomOut::before {
            content: url(images/toolbarButton-zoomOut.png)
        }

        .pdfjs .toolbarButton.zoomIn::before {
            content: url(images/toolbarButton-zoomIn.png)
        }

        .pdfjs .toolbarButton.presentationMode::before,
        .pdfjs .secondaryToolbarButton.presentationMode::before {
            content: url(images/toolbarButton-presentationMode.png)
        }

        .pdfjs .toolbarButton.print::before,
        .pdfjs .secondaryToolbarButton.print::before {
            content: url(images/toolbarButton-print.png)
        }

        .pdfjs .toolbarButton.openFile::before,
        .pdfjs .secondaryToolbarButton.openFile::before {
            content: url(images/toolbarButton-openFile.png)
        }

        .pdfjs .toolbarButton.download::before,
        .pdfjs .secondaryToolbarButton.download::before {
            content: url(images/toolbarButton-download.png)
        }

        .pdfjs .toolbarButton.bookmark,
        .pdfjs .secondaryToolbarButton.bookmark {
            -webkit-box-sizing: border-box;
            -moz-box-sizing: border-box;
            box-sizing: border-box;
            outline: none;
            padding-top: 4px;
            text-decoration: none
        }

        .pdfjs .secondaryToolbarButton.bookmark {
            padding-top: 5px
        }

        .pdfjs .bookmark[href='#'] {
            opacity: .5;
            pointer-events: none
        }

        .pdfjs .toolbarButton.bookmark::before,
        .pdfjs .secondaryToolbarButton.bookmark::before {
            content: url(images/toolbarButton-bookmark.png)
        }

        .pdfjs #viewThumbnail.toolbarButton::before {
            content: url(images/toolbarButton-viewThumbnail.png)
        }

        html[dir="ltr"] .pdfjs #viewOutline.toolbarButton::before {
            content: url(images/toolbarButton-viewOutline.png)
        }

        html[dir="rtl"] .pdfjs #viewOutline.toolbarButton::before {
            content: url(images/toolbarButton-viewOutline-rtl.png)
        }

        .pdfjs #viewAttachments.toolbarButton::before {
            content: url(images/toolbarButton-viewAttachments.png)
        }

        .pdfjs #viewFind.toolbarButton::before {
            content: url(images/toolbarButton-search.png)
        }

        .pdfjs .secondaryToolbarButton {
            position: relative;
            margin: 0 0 4px;
            padding: 3px 0 1px;
            height: auto;
            min-height: 25px;
            width: auto;
            min-width: 100%;
            white-space: normal
        }

        html[dir="ltr"] .pdfjs .secondaryToolbarButton {
            padding-left: 24px;
            text-align: left
        }

        html[dir="rtl"] .pdfjs .secondaryToolbarButton {
            padding-right: 24px;
            text-align: right
        }

        html[dir="ltr"] .pdfjs .secondaryToolbarButton.bookmark {
            padding-left: 27px
        }

        html[dir="rtl"] .pdfjs .secondaryToolbarButton.bookmark {
            padding-right: 27px
        }

        html[dir="ltr"] .pdfjs .secondaryToolbarButton>span {
            padding-right: 4px
        }

        html[dir="rtl"] .pdfjs .secondaryToolbarButton>span {
            padding-left: 4px
        }

        .pdfjs .secondaryToolbarButton.firstPage::before {
            content: url(images/secondaryToolbarButton-firstPage.png)
        }

        .pdfjs .secondaryToolbarButton.lastPage::before {
            content: url(images/secondaryToolbarButton-lastPage.png)
        }

        .pdfjs .secondaryToolbarButton.rotateCcw::before {
            content: url(images/secondaryToolbarButton-rotateCcw.png)
        }

        .pdfjs .secondaryToolbarButton.rotateCw::before {
            content: url(images/secondaryToolbarButton-rotateCw.png)
        }

        .pdfjs .secondaryToolbarButton.handTool::before {
            content: url(images/secondaryToolbarButton-handTool.png)
        }

        .pdfjs .secondaryToolbarButton.documentProperties::before {
            content: url(images/secondaryToolbarButton-documentProperties.png)
        }

        .pdfjs .verticalToolbarSeparator {
            display: block;
            padding: 8px 0;
            margin: 8px 4px;
            width: 1px;
            background-color: hsla(0, 0%, 0%, .5);
            box-shadow: 0 0 0 1px hsla(0, 0%, 100%, .08)
        }

        html[dir='ltr'] .pdfjs .verticalToolbarSeparator {
            margin-left: 2px
        }

        html[dir='rtl'] .pdfjs .verticalToolbarSeparator {
            margin-right: 2px
        }

        .pdfjs .horizontalToolbarSeparator {
            display: block;
            margin: 0 0 4px;
            height: 1px;
            width: 100%;
            background-color: hsla(0, 0%, 0%, .5);
            box-shadow: 0 0 0 1px hsla(0, 0%, 100%, .08)
        }

        .pdfjs .toolbarField {
            padding: 3px 6px;
            margin: 4px 0;
            border: 1px solid transparent;
            border-radius: 2px;
            background-color: hsla(0, 0%, 100%, .09);
            background-image: linear-gradient(hsla(0, 0%, 100%, .05), hsla(0, 0%, 100%, 0));
            background-clip: padding-box;
            border: 1px solid hsla(0, 0%, 0%, .35);
            border-color: hsla(0, 0%, 0%, .32) hsla(0, 0%, 0%, .38) hsla(0, 0%, 0%, .42);
            box-shadow: 0 1px 0 hsla(0, 0%, 0%, .05) inset, 0 1px 0 hsla(0, 0%, 100%, .05);
            color: hsl(0, 0%, 95%);
            font-size: 12px;
            line-height: 14px;
            outline-style: none;
            transition-property: background-color, border-color, box-shadow;
            transition-duration: 150ms;
            transition-timing-function: ease
        }

        .pdfjs .toolbarField[type=checkbox] {
            display: inline-block;
            margin: 8px 0
        }

        .pdfjs .toolbarField.pageNumber {
            -moz-appearance: textfield;
            min-width: 16px;
            text-align: right;
            width: 40px
        }

        .pdfjs .toolbarField.pageNumber.visiblePageIsLoading {
            background-image: url(images/loading-small.png);
            background-repeat: no-repeat;
            background-position: 1px
        }

        .pdfjs .toolbarField.pageNumber::-webkit-inner-spin-button,
        .pdfjs .toolbarField.pageNumber::-webkit-outer-spin-button {
            -webkit-appearance: none;
            margin: 0
        }

        .pdfjs .toolbarField:hover {
            background-color: hsla(0, 0%, 100%, .11);
            border-color: hsla(0, 0%, 0%, .4) hsla(0, 0%, 0%, .43) hsla(0, 0%, 0%, .45)
        }

        .pdfjs .toolbarField:focus {
            background-color: hsla(0, 0%, 100%, .15);
            border-color: hsla(204, 100%, 65%, .8) hsla(204, 100%, 65%, .85) hsla(204, 100%, 65%, .9)
        }

        .pdfjs .toolbarLabel {
            min-width: 16px;
            padding: 3px 6px 3px 2px;
            margin: 4px 2px 4px 0;
            border: 1px solid transparent;
            border-radius: 2px;
            color: hsl(0, 0%, 85%);
            font-size: 12px;
            line-height: 14px;
            text-align: left;
            -webkit-user-select: none;
            -moz-user-select: none;
            cursor: default
        }

        .pdfjs #thumbnailView {
            position: absolute;
            width: 120px;
            top: 0;
            bottom: 0;
            padding: 10px 40px 0;
            overflow: auto;
            -webkit-overflow-scrolling: touch
        }

        .pdfjs .thumbnail {
            float: left;
            margin-bottom: 5px
        }

        .pdfjs #thumbnailView>a:last-of-type>.thumbnail {
            margin-bottom: 10px
        }

        .pdfjs #thumbnailView>a:last-of-type>.thumbnail:not([data-loaded]) {
            margin-bottom: 9px
        }

        .pdfjs .thumbnail:not([data-loaded]) {
            border: 1px dashed rgba(255, 255, 255, 0.5);
            margin: -1px -1px 4px
        }

        .pdfjs .thumbnailImage {
            border: 1px solid transparent;
            box-shadow: 0 0 0 1px rgba(0, 0, 0, 0.5), 0 2px 8px rgba(0, 0, 0, 0.3);
            opacity: .8;
            z-index: 99;
            background-color: #fff;
            background-clip: content-box
        }

        .pdfjs .thumbnailSelectionRing {
            border-radius: 2px;
            padding: 7px
        }

        .pdfjs a:focus>.thumbnail>.thumbnailSelectionRing>.thumbnailImage,
        .pdfjs .thumbnail:hover>.thumbnailSelectionRing>.thumbnailImage {
            opacity: .9
        }

        .pdfjs a:focus>.thumbnail>.thumbnailSelectionRing,
        .pdfjs .thumbnail:hover>.thumbnailSelectionRing {
            background-color: hsla(0, 0%, 100%, .15);
            background-image: linear-gradient(hsla(0, 0%, 100%, .05), hsla(0, 0%, 100%, 0));
            background-clip: padding-box;
            box-shadow: 0 1px 0 hsla(0, 0%, 100%, .05) inset, 0 0 1px hsla(0, 0%, 100%, .2) inset, 0 0 1px hsla(0, 0%, 0%, .2);
            color: hsla(0, 0%, 100%, .9)
        }

        .pdfjs .thumbnail.selected>.thumbnailSelectionRing>.thumbnailImage {
            box-shadow: 0 0 0 1px hsla(0, 0%, 0%, .5);
            opacity: 1
        }

        .pdfjs .thumbnail.selected>.thumbnailSelectionRing {
            background-color: hsla(0, 0%, 100%, .3);
            background-image: linear-gradient(hsla(0, 0%, 100%, .05), hsla(0, 0%, 100%, 0));
            background-clip: padding-box;
            box-shadow: 0 1px 0 hsla(0, 0%, 100%, .05) inset, 0 0 1px hsla(0, 0%, 100%, .1) inset, 0 0 1px hsla(0, 0%, 0%, .2);
            color: hsla(0, 0%, 100%, 1)
        }

        .pdfjs #outlineView,
        .pdfjs #attachmentsView {
            position: absolute;
            width: 192px;
            top: 0;
            bottom: 0;
            overflow: auto;
            -webkit-overflow-scrolling: touch;
            -webkit-user-select: none;
            -moz-user-select: none
        }

        .pdfjs #outlineView {
            padding: 4px 4px 0
        }

        .pdfjs #attachmentsView {
            padding: 3px 4px 0
        }

        html[dir='ltr'] .pdfjs .outlineItem>.outlineItems {
            margin-left: 20px
        }

        html[dir='rtl'] .pdfjs .outlineItem>.outlineItems {
            margin-right: 20px
        }

        .pdfjs .outlineItem>a,
        .pdfjs .attachmentsItem>button {
            text-decoration: none;
            display: inline-block;
            min-width: 95%;
            height: auto;
            margin-bottom: 1px;
            border-radius: 2px;
            color: hsla(0, 0%, 100%, .8);
            font-size: 13px;
            line-height: 15px;
            -moz-user-select: none;
            white-space: normal
        }

        .pdfjs .attachmentsItem>button {
            border: 0 none;
            background: none;
            cursor: pointer;
            width: 100%
        }

        html[dir='ltr'] .pdfjs .outlineItem>a {
            padding: 2px 0 5px 10px
        }

        html[dir='ltr'] .pdfjs .attachmentsItem>button {
            padding: 2px 0 3px 7px;
            text-align: left
        }

        html[dir='rtl'] .pdfjs .outlineItem>a {
            padding: 2px 10px 5px 0
        }

        html[dir='rtl'] .pdfjs .attachmentsItem>button {
            padding: 2px 7px 3px 0;
            text-align: right
        }

        .pdfjs .outlineItem>a:hover,
        .pdfjs .attachmentsItem>button:hover {
            background-color: hsla(0, 0%, 100%, .02);
            background-image: linear-gradient(hsla(0, 0%, 100%, .05), hsla(0, 0%, 100%, 0));
            background-clip: padding-box;
            box-shadow: 0 1px 0 hsla(0, 0%, 100%, .05) inset, 0 0 1px hsla(0, 0%, 100%, .2) inset, 0 0 1px hsla(0, 0%, 0%, .2);
            color: hsla(0, 0%, 100%, .9)
        }

        .pdfjs .outlineItem.selected {
            background-color: hsla(0, 0%, 100%, .08);
            background-image: linear-gradient(hsla(0, 0%, 100%, .05), hsla(0, 0%, 100%, 0));
            background-clip: padding-box;
            box-shadow: 0 1px 0 hsla(0, 0%, 100%, .05) inset, 0 0 1px hsla(0, 0%, 100%, .1) inset, 0 0 1px hsla(0, 0%, 0%, .2);
            color: hsla(0, 0%, 100%, 1)
        }

        .pdfjs .noResults {
            font-size: 12px;
            color: hsla(0, 0%, 100%, .8);
            font-style: italic;
            cursor: default
        }

        .pdfjs ::selection {
            background: rgba(0, 0, 255, 0.3)
        }

        .pdfjs ::-moz-selection {
            background: rgba(0, 0, 255, 0.3)
        }

        .pdfjs #errorWrapper {
            background: none repeat scroll 0 0 #F55;
            color: #fff;
            left: 0;
            position: absolute;
            right: 0;
            z-index: 1000;
            padding: 3px;
            font-size: .8em
        }

        .pdfjs .loadingInProgress #errorWrapper {
            top: 37px
        }

        .pdfjs #errorMessageLeft {
            float: left
        }

        .pdfjs #errorMessageRight {
            float: right
        }

        .pdfjs #errorMoreInfo {
            background-color: #FFF;
            color: #000;
            padding: 3px;
            margin: 3px;
            width: 98%
        }

        .pdfjs .overlayButton {
            width: auto;
            margin: 3px 4px 2px!important;
            padding: 2px 6px 3px
        }

        .pdfjs #overlayContainer {
            display: table;
            position: absolute;
            width: 100%;
            height: 100%;
            background-color: hsla(0, 0%, 0%, .2);
            z-index: 40000
        }

        .pdfjs #overlayContainer>* {
            overflow: auto;
            -webkit-overflow-scrolling: touch
        }

        .pdfjs #overlayContainer>.container {
            display: table-cell;
            vertical-align: middle;
            text-align: center
        }

        .pdfjs #overlayContainer>.container>.dialog {
            display: inline-block;
            padding: 15px;
            border-spacing: 4px;
            color: hsl(0, 0%, 85%);
            font-size: 12px;
            line-height: 14px;
            background-color: #474747;
            background-image: url(images/texture.png), linear-gradient(hsla(0, 0%, 32%, .99), hsla(0, 0%, 27%, .95));
            box-shadow: inset 1px 0 0 hsla(0, 0%, 100%, .08), inset 0 1px 1px hsla(0, 0%, 0%, .15), inset 0 -1px 0 hsla(0, 0%, 100%, .05), 0 1px 0 hsla(0, 0%, 0%, .15), 0 1px 1px hsla(0, 0%, 0%, .1);
            border: 1px solid hsla(0, 0%, 0%, .5);
            border-radius: 4px;
            box-shadow: 0 1px 4px rgba(0, 0, 0, 0.3)
        }

        .pdfjs .dialog>.row {
            display: table-row
        }

        .pdfjs .dialog>.row>* {
            display: table-cell
        }

        .pdfjs .dialog .toolbarField {
            margin: 5px 0
        }

        .pdfjs .dialog .separator {
            display: block;
            margin: 4px 0;
            height: 1px;
            width: 100%;
            background-color: hsla(0, 0%, 0%, .5);
            box-shadow: 0 0 0 1px hsla(0, 0%, 100%, .08)
        }

        .pdfjs .dialog .buttonRow {
            text-align: center;
            vertical-align: middle
        }

        .pdfjs #passwordOverlay>.dialog {
            text-align: center
        }

        .pdfjs #passwordOverlay .toolbarField {
            width: 200px
        }

        .pdfjs #documentPropertiesOverlay>.dialog {
            text-align: left
        }

        .pdfjs #documentPropertiesOverlay .row>* {
            min-width: 100px
        }

        html[dir='ltr'] .pdfjs #documentPropertiesOverlay .row>* {
            text-align: left
        }

        html[dir='rtl'] .pdfjs #documentPropertiesOverlay .row>* {
            text-align: right
        }

        .pdfjs #documentPropertiesOverlay .row>span {
            width: 125px;
            word-wrap: break-word
        }

        .pdfjs #documentPropertiesOverlay .row>p {
            max-width: 225px;
            word-wrap: break-word
        }

        .pdfjs #documentPropertiesOverlay .buttonRow {
            margin-top: 10px
        }

        .pdfjs .clearBoth {
            clear: both
        }

        .pdfjs .fileInput {
            background: #fff;
            color: #000;
            margin-top: 5px;
            visibility: hidden;
            position: fixed;
            right: 0;
            top: 0
        }

        .pdfjs #PDFBug {
            background: none repeat scroll 0 0 #fff;
            border: 1px solid #666;
            position: fixed;
            top: 32px;
            right: 0;
            bottom: 0;
            font-size: 10px;
            padding: 0;
            width: 300px
        }

        .pdfjs #PDFBug .controls {
            background: #EEE;
            border-bottom: 1px solid #666;
            padding: 3px
        }

        .pdfjs #PDFBug .panels {
            bottom: 0;
            left: 0;
            overflow: auto;
            -webkit-overflow-scrolling: touch;
            position: absolute;
            right: 0;
            top: 27px
        }

        .pdfjs #PDFBug button.active {
            font-weight: 700
        }

        .pdfjs .debuggerShowText {
            background: none repeat scroll 0 0 #ff0;
            color: blue
        }

        .pdfjs .debuggerHideText:hover {
            background: none repeat scroll 0 0 #ff0
        }

        .pdfjs #PDFBug .stats {
            font-family: courier;
            font-size: 10px;
            white-space: pre
        }

        .pdfjs #PDFBug .stats .title {
            font-weight: 700
        }

        .pdfjs #PDFBug table {
            font-size: 10px
        }

        .pdfjs #viewer.textLayer-visible .textLayer>div,
        .pdfjs #viewer.textLayer-hover .textLayer>div:hover {
            background-color: #fff;
            color: #000
        }

        .pdfjs #viewer.textLayer-shadow .textLayer>div {
            background-color: rgba(255, 255, 255, .6);
            color: #000
        }

        .pdfjs .grab-to-pan-grab {
            cursor: url(images/grab.cur), move!important;
            cursor: -webkit-grab!important;
            cursor: -moz-grab!important;
            cursor: grab!important
        }

        .pdfjs .grab-to-pan-grab :not(input):not(textarea):not(button):not(select):not(:link) {
            cursor: inherit!important
        }

        .pdfjs .grab-to-pan-grab:active,
        .pdfjs .grab-to-pan-grabbing {
            cursor: url(images/grabbing.cur), move!important;
            cursor: -webkit-grabbing!important;
            cursor: -moz-grabbing!important;
            cursor: grabbing!important;
            position: fixed;
            background: transparent;
            display: block;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            overflow: hidden;
            z-index: 50000
        }

        @page {
            margin: 0
        }

        .pdfjs #printContainer {
            display: none
        }

        @media screen and (min-resolution: 2dppx) {
            .pdfjs .toolbarButton::before {
                -webkit-transform: scale(0.5);
                transform: scale(0.5);
                top: -5px
            }
            .pdfjs .secondaryToolbarButton::before {
                -webkit-transform: scale(0.5);
                transform: scale(0.5);
                top: -4px
            }
            html[dir='ltr'] .pdfjs .toolbarButton::before,
            html[dir='rtl'] .pdfjs .toolbarButton::before {
                left: -1px
            }
            html[dir='ltr'] .pdfjs .secondaryToolbarButton::before {
                left: -2px
            }
            html[dir='rtl'] .pdfjs .secondaryToolbarButton::before {
                left: 186px
            }
            .pdfjs .toolbarField.pageNumber.visiblePageIsLoading,
            .pdfjs #findInput[data-status="pending"] {
                background-image: url(images/loading-small@2x.png);
                background-size: 16px 17px
            }
            .pdfjs .dropdownToolbarButton {
                background: url(images/toolbarButton-menuArrows@2x.png) no-repeat;
                background-size: 7px 16px
            }
            html[dir='ltr'] .pdfjs .toolbarButton#sidebarToggle::before {
                content: url(images/toolbarButton-sidebarToggle@2x.png)
            }
            html[dir='rtl'] .pdfjs .toolbarButton#sidebarToggle::before {
                content: url(images/toolbarButton-sidebarToggle-rtl@2x.png)
            }
            html[dir='ltr'] .pdfjs .toolbarButton#secondaryToolbarToggle::before {
                content: url(images/toolbarButton-secondaryToolbarToggle@2x.png)
            }
            html[dir='rtl'] .pdfjs .toolbarButton#secondaryToolbarToggle::before {
                content: url(images/toolbarButton-secondaryToolbarToggle-rtl@2x.png)
            }
            html[dir='ltr'] .pdfjs .toolbarButton.findPrevious::before {
                content: url(images/findbarButton-previous@2x.png)
            }
            html[dir='rtl'] .pdfjs .toolbarButton.findPrevious::before {
                content: url(images/findbarButton-previous-rtl@2x.png)
            }
            html[dir='ltr'] .pdfjs .toolbarButton.findNext::before {
                content: url(images/findbarButton-next@2x.png)
            }
            html[dir='rtl'] .pdfjs .toolbarButton.findNext::before {
                content: url(images/findbarButton-next-rtl@2x.png)
            }
            html[dir='ltr'] .pdfjs .toolbarButton.pageUp::before {
                content: url(images/toolbarButton-pageUp@2x.png)
            }
            html[dir='rtl'] .pdfjs .toolbarButton.pageUp::before {
                content: url(images/toolbarButton-pageUp-rtl@2x.png)
            }
            html[dir='ltr'] .pdfjs .toolbarButton.pageDown::before {
                content: url(images/toolbarButton-pageDown@2x.png)
            }
            html[dir='rtl'] .pdfjs .toolbarButton.pageDown::before {
                content: url(images/toolbarButton-pageDown-rtl@2x.png)
            }
            .pdfjs .toolbarButton.zoomIn::before {
                content: url(images/toolbarButton-zoomIn@2x.png)
            }
            .pdfjs .toolbarButton.zoomOut::before {
                content: url(images/toolbarButton-zoomOut@2x.png)
            }
            .pdfjs .toolbarButton.presentationMode::before,
            .pdfjs .secondaryToolbarButton.presentationMode::before {
                content: url(images/toolbarButton-presentationMode@2x.png)
            }
            .pdfjs .toolbarButton.print::before,
            .pdfjs .secondaryToolbarButton.print::before {
                content: url(images/toolbarButton-print@2x.png)
            }
            .pdfjs .toolbarButton.openFile::before,
            .pdfjs .secondaryToolbarButton.openFile::before {
                content: url(images/toolbarButton-openFile@2x.png)
            }
            .pdfjs .toolbarButton.download::before,
            .pdfjs .secondaryToolbarButton.download::before {
                content: url(images/toolbarButton-download@2x.png)
            }
            .pdfjs .toolbarButton.bookmark::before,
            .pdfjs .secondaryToolbarButton.bookmark::before {
                content: url(images/toolbarButton-bookmark@2x.png)
            }
            .pdfjs #viewThumbnail.toolbarButton::before {
                content: url(images/toolbarButton-viewThumbnail@2x.png)
            }
            html[dir="ltr"] .pdfjs #viewOutline.toolbarButton::before {
                content: url(images/toolbarButton-viewOutline@2x.png)
            }
            html[dir="rtl"] .pdfjs #viewOutline.toolbarButton::before {
                content: url(images/toolbarButton-viewOutline-rtl@2x.png)
            }
            .pdfjs #viewAttachments.toolbarButton::before {
                content: url(images/toolbarButton-viewAttachments@2x.png)
            }
            .pdfjs #viewFind.toolbarButton::before {
                content: url(images/toolbarButton-search@2x.png)
            }
            .pdfjs .secondaryToolbarButton.firstPage::before {
                content: url(images/secondaryToolbarButton-firstPage@2x.png)
            }
            .pdfjs .secondaryToolbarButton.lastPage::before {
                content: url(images/secondaryToolbarButton-lastPage@2x.png)
            }
            .pdfjs .secondaryToolbarButton.rotateCcw::before {
                content: url(images/secondaryToolbarButton-rotateCcw@2x.png)
            }
            .pdfjs .secondaryToolbarButton.rotateCw::before {
                content: url(images/secondaryToolbarButton-rotateCw@2x.png)
            }
            .pdfjs .secondaryToolbarButton.handTool::before {
                content: url(images/secondaryToolbarButton-handTool@2x.png)
            }
            .pdfjs .secondaryToolbarButton.documentProperties::before {
                content: url(images/secondaryToolbarButton-documentProperties@2x.png)
            }
        }

        @media print {
            body {
                background: transparent none
            }
            .pdfjs #sidebarContainer,
            .pdfjs #secondaryToolbar,
            .pdfjs .toolbar,
            .pdfjs #loadingBox,
            .pdfjs #errorWrapper,
            .pdfjs .textLayer {
                display: none
            }
            .pdfjs #viewerContainer {
                overflow: visible
            }
            .pdfjs #mainContainer,
            .pdfjs #viewerContainer,
            .pdfjs .page,
            .pdfjs .page canvas {
                position: static;
                padding: 0;
                margin: 0
            }
            .pdfjs .page {
                float: left;
                display: none;
                border: none;
                box-shadow: none;
                background-clip: content-box;
                background-color: #fff
            }
            .pdfjs .page[data-loaded] {
                display: block
            }
            .pdfjs .fileInput {
                display: none
            }
            body[data-mozPrintCallback] .pdfjs #outerContainer {
                display: none
            }
            body[data-mozPrintCallback] .pdfjs #printContainer {
                display: block
            }
            .pdfjs #printContainer>div {
                position: relative;
                top: 0;
                left: 0;
                overflow: hidden
            }
            .pdfjs #printContainer canvas {
                display: block
            }
        }

        .pdfjs .visibleLargeView,
        .pdfjs .visibleMediumView,
        .pdfjs .visibleSmallView {
            display: none
        }

        @media all and (max-width: 960px) {
            html[dir='ltr'] .pdfjs #outerContainer.sidebarMoving .outerCenter,
            html[dir='ltr'] .pdfjs #outerContainer.sidebarOpen .outerCenter {
                float: left;
                left: 205px
            }
            html[dir='rtl'] .pdfjs #outerContainer.sidebarMoving .outerCenter,
            html[dir='rtl'] .pdfjs #outerContainer.sidebarOpen .outerCenter {
                float: right;
                right: 205px
            }
        }

        @media all and (max-width: 900px) {
            .pdfjs .sidebarOpen .hiddenLargeView {
                display: none
            }
            .pdfjs .sidebarOpen .visibleLargeView {
                display: inherit
            }
        }

        @media all and (max-width: 860px) {
            .pdfjs .sidebarOpen .hiddenMediumView {
                display: none
            }
            .pdfjs .sidebarOpen .visibleMediumView {
                display: inherit
            }
        }

        @media all and (max-width: 770px) {
            .pdfjs #sidebarContainer {
                top: 32px;
                z-index: 100
            }
            .pdfjs .loadingInProgress #sidebarContainer {
                top: 37px
            }
            .pdfjs #sidebarContent {
                top: 32px;
                background-color: hsla(0, 0%, 0%, .7)
            }
            html[dir='ltr'] .pdfjs #outerContainer.sidebarOpen>#mainContainer {
                left: 0
            }
            html[dir='rtl'] .pdfjs #outerContainer.sidebarOpen>#mainContainer {
                right: 0
            }
            html[dir='ltr'] .pdfjs .outerCenter {
                float: left;
                left: 205px
            }
            html[dir='rtl'] .pdfjs .outerCenter {
                float: right;
                right: 205px
            }
            .pdfjs #outerContainer .hiddenLargeView,
            .pdfjs #outerContainer .hiddenMediumView {
                display: inherit
            }
            .pdfjs #outerContainer .visibleLargeView,
            .pdfjs #outerContainer .visibleMediumView {
                display: none
            }
        }

        @media all and (max-width: 700px) {
            .pdfjs #outerContainer .hiddenLargeView {
                display: none
            }
            .pdfjs #outerContainer .visibleLargeView {
                display: inherit
            }
        }

        @media all and (max-width: 660px) {
            .pdfjs #outerContainer .hiddenMediumView {
                display: none
            }
            .pdfjs #outerContainer .visibleMediumView {
                display: inherit
            }
        }

        @media all and (max-width: 600px) {
            .pdfjs .hiddenSmallView {
                display: none
            }
            .pdfjs .visibleSmallView {
                display: inherit
            }
            html[dir='ltr'] .pdfjs #outerContainer.sidebarMoving .outerCenter,
            html[dir='ltr'] .pdfjs #outerContainer.sidebarOpen .outerCenter,
            html[dir='ltr'] .pdfjs .outerCenter {
                left: 156px
            }
            html[dir='rtl'] .pdfjs #outerContainer.sidebarMoving .outerCenter,
            html[dir='rtl'] .pdfjs #outerContainer.sidebarOpen .outerCenter,
            html[dir='rtl'] .pdfjs .outerCenter {
                right: 156px
            }
            .pdfjs .toolbarButtonSpacer {
                width: 0
            }
        }

        @media all and (max-width: 510px) {
            .pdfjs #scaleSelectContainer,
            .pdfjs #pageNumberLabel {
                display: none
            }
        }


            /* should be hidden differently */
            #fileInput.fileInput {display: none;}

</style>




