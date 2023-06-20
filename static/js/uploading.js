let uploaded_filenameid = null;

const Toast = Swal.mixin({
    toast: true,
    position: 'center-center',
    showConfirmButton: false,
    timer: 1500,
    timerProgressBar: true,
    didOpen: (toast) => {
        toast.addEventListener('mouseenter', Swal.stopTimer)
        toast.addEventListener('mouseleave', Swal.resumeTimer)
    }
})

class FileUpload {
    constructor(input) {
        this.input = input
        this.max_length = 1024 * 1024 * 10; // 10 mb
    }

    upload() {
        this.create_progress_bar();
        this.initFileUpload();
    }

    initFileUpload() {
        this.file = this.input.files[0];
        this.upload_file(0, null);
    }

    upload_file(start, path) {
        var end;
        var self = this;
        var existingPath = path;
        var formData = new FormData();
        var nextChunk = start + this.max_length + 1;
        var currentChunk = this.file.slice(start, nextChunk);
        var uploadedChunk = start + currentChunk.size

        if (uploadedChunk >= this.file.size) {
            end = 1;
        } else {
            end = 0;
        }

        formData.append('file', currentChunk);
        formData.append('filename', this.file.name);
        formData.append('end', end);
        formData.append('existingPath', existingPath);
        formData.append('nextSlice', nextChunk);
        $('.textbox').text("Uploading file")
        $.ajaxSetup({
        // make sure to send the header
            headers: {
                "X-CSRFToken": document.querySelector('[name=csrfmiddlewaretoken]').value,
            }
        });
        $.ajax({
            xhr: function () {
                var xhr = new XMLHttpRequest();
                xhr.upload.addEventListener('progress', function (e) {
                    if (e.lengthComputable) {
                        if (self.file.size < self.max_length) {
                            var percent = Math.round((e.loaded / e.total) * 100);
                        } else {
                            var percent = Math.round((uploadedChunk / self.file.size) * 100);
                        }
                        $('.progress-bar').css('width', percent + '%')
                        $('.progress-bar').text(percent + '%')
                    }
                });
                return xhr;
            },
            url: '/detect/upload/',
            type: 'POST',
            dataType: 'json',
            cache: false,
            processData: false,
            contentType: false,
            data: formData,
            error: function (xhr) {
                alert(xhr.statusText);
            },
            success: function (res) {
                if (nextChunk < self.file.size) {
                    // upload file in chunks
                    existingPath = res.existingPath
                    self.upload_file(nextChunk, existingPath);
                } else {
                    // upload complete
                    $('.textbox').text(res.data);
                    uploaded_filenameid = res.fileid;

                    $('#detecting').attr('disabled', false);
                    $('#detecting').attr('hidden', false);
                }
                Toast.fire({
                    icon: 'success',
                    title: 'Uploaded Successfully'
                })

            }
        });
    };

    create_progress_bar() {

        document.getElementById('fileupload').setAttribute('disabled',"")
        document.getElementById('submit-file').setAttribute('hidden',"")
        document.getElementById('submit-file').setAttribute('disabled',"")

        var progress = `<div class="file-details">
                            <small class="fileid_id"></small>
                            <small class="textbox"></small>
                            <div class="progress" style="margin-top: 5px;">
                                <div class="progress-bar bg-success" role="progressbar" aria-valuenow="0" aria-valuemin="0" aria-valuemax="100" style="width: 0%">
                                </div>
                            </div>
                        </div>`
        document.getElementById('uploaded_files').innerHTML = progress
    }

}

(function ($) {
    $('#submit-file').on('click', (event) => {
        event.preventDefault();

        var fileVal = $("#fileupload").val();
        var maxSize = 10 * 1024 * 1024; // 10MB
        var fileSize = $("#fileupload")[0].files[0].size;

        // 파일 업로드 확장자 체크
        if (fileVal != "") {
            var ext = fileVal.split('.').pop().toLowerCase(); //확장자분리
            //아래 확장자가 있는지 체크
            if ($.inArray(ext, ['c', 'cc', 'cpp']) == -1) {
                Toast.fire({
                    icon: 'error',
                    title: ' ".c, .cc, .cpp" 파일만 업로드 가능합니다.'
                });
                return false;
            }
        }
        // 파일 업로드 용량 체크
        if (fileSize > maxSize) {
            Toast.fire({
                icon: 'error',
                title: "첨부파일 사이즈는 10MB 이내로 등록 가능합니다."
            });
            $("#fileupload").val("");
            return false;
        }

        var uploader = new FileUpload(document.querySelector('#fileupload'))
        uploader.upload();
    });
})(jQuery);






