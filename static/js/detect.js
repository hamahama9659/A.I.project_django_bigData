
(function ($) {
    $('#detecting').on('click', () => {
        var formData = new FormData();
        formData.append('fileid', uploaded_filenameid);

        // remove fileid
        uploaded_filenameid = null;

        $.ajax({
            url: '/detect/beforepredict/',
            type: 'POST',
            dataType: 'json',
            cache: false,
            processData: false,
            contentType: false,
            data: formData,
            error: function () {
                alert("Error : Please Retry");
            },
            success: function (jres) {
                // 1. ag-grid list 불러오는 AJAX function 실행
                gridOptions.api.setRowData('');
                vulInfo = JSON.parse(jres.vuls_js);
                gridOptions.api.setRowData(vulInfo);

                // 2. Close Modal popup
                var modal = document.getElementById('myModal');
                modal.style.display = "none";
                // Refresh Modal popup when close
                $('.file-details').detach();
                $('#fileupload').removeAttr('disabled');
                $('#submit-file').removeAttr('disabled');
                $('#submit-file').removeAttr('hidden');
                $('#fileupload').val("");
                $('#detecting').attr('disabled', true);
                $('#detecting').attr('hidden', true);
            },
            complete: function (jres) {
                // 3. list 불러오면서 새로운 AJAX function 실행해서 분석 시작시키기
                var keyid = formData.get('fileid');
                predicting(keyid);
            },
        });
    });
})(jQuery);



function predicting(keyid) {
    var fileData = new FormData();
    fileData.append('fileid', keyid);

    $.ajax({
        type: 'POST',
        url: '/detect/startpredict/',
        dataType: 'json',
        cache: false,
        processData: false,
        contentType: false,
        data: fileData,
        success: function(jres) {
            gridOptions.api.setRowData('');
            vulInfo = JSON.parse(jres.vuls_js);
            gridOptions.api.setRowData(vulInfo);
        },
        complete: function (jres) {
            Toast.fire({
                icon: 'success',
                title: 'Finished detecting'
            })
        },
    });
}
