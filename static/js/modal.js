// Get the modal
var modal = document.getElementById('myModal');

// Get the button that opens the modal
var btn = document.getElementById("myBtn");

// Get the <span> element that closes the modal
var span = document.getElementById("closeModal");

// When the user clicks on the button, open the modal
btn.onclick = function() {
    modal.style.display = "block";
}

// When the user clicks on <span> (x), close the modal
span.onclick = function() {
    modal.style.display = "none";

    // Refresh when close Modal popup
    $('.file-details').detach();
    $('#fileupload').removeAttr('disabled');
    $('#submit-file').removeAttr('disabled');
    $('#submit-file').removeAttr('hidden');
    $('#fileupload').val("");
    $('#detecting').attr('disabled', true);
    $('#detecting').attr('hidden', true);

    // Delete file data in DB
    closingmodal();
}

// When the user clicks anywhere outside of the modal, close it
window.onclick = function(event) {
    if (event.target == modal) {
        modal.style.display = "none";

        // Refresh when close Modal popup
        $('.file-details').detach();
        $('#fileupload').removeAttr('disabled');
        $('#submit-file').removeAttr('disabled');
        $('#submit-file').removeAttr('hidden');
        $('#fileupload').val("");
        $('#detecting').attr('disabled', true);
        $('#detecting').attr('hidden', true);

        // Delete file data in DB
        closingmodal();
    }
}



function closingmodal() {
    var fileData = new FormData();
    fileData.append('fileid', uploaded_filenameid);

    $.ajax({
        type: 'POST',
        url: '/detect/modalclose/',
        dataType: 'json',
        cache: false,
        processData: false,
        contentType: false,
        data: fileData,
        success: function(res) {
            // remove fileid
            uploaded_filenameid = null;

            // alert message
            Toast.fire({
                icon: 'success',
                title: res.data
            })
        }

    });
}