
// cellRenderer : gage bar for percent
var ragRenderer = (params) => {
    var nums = parseFloat(params.value * 100).toFixed(2);
    var gage = nums + '%';

    // for Percent bar CSS
    if (params.value == 2) {
        return '<div class="rag-gif"><img src="' + '/static/img/detecting-wave.gif' + '" width="auto" height="100%"></div>';
    } else if (params.value < 0.3) {
        return '<div class="rag-div" style="width:' + gage + ';"><div class="rag-green";>' + '&nbsp;&nbsp;' + gage + '</div></div>';
    } else if (params.value >= 0.3 && params.value < 0.8) {
        return '<div class="rag-div" style="width:' + gage + ';"><div class="rag-amber";>' + '&nbsp;&nbsp;' + gage + '</div></div>';
    } else {
        return '<div class="rag-div" style="width:' + gage + ';"><div class="rag-red";>' + '&nbsp;&nbsp;' + gage + '</div></div>';
    }
};

// columnDefs : about columns
var columnDefs = [
    { headerName: "NAME", field: "filename", pinned: 'left'},
    { headerName: "분석시작", field: "operatetime", suppressSizeToFit: true},
    { headerName: "분석종료", field: "endtime", suppressSizeToFit: true},
    { headerName: "CWE-119" , field: "cwe119", cellRenderer: ragRenderer, flex:1},
    { headerName: "CWE-120" , field: "cwe120", cellRenderer: ragRenderer, flex:1},
    { headerName: "CWE-469" , field: "cwe469", cellRenderer: ragRenderer, flex:1},
    { headerName: "CWE-476" , field: "cwe476", cellRenderer: ragRenderer, flex:1},
    { headerName: "CWE-OTHER" , field: "cweother", cellRenderer: ragRenderer, flex:1},
];


// Keyboard up & down
function navigateToNextCell(params) {
    var suggestedNextCell = params.nextCellPosition;

    var KEY_UP = 38;
    var KEY_DOWN = 40;

    var noUpOrDownKeyPressed = params.key !== KEY_DOWN && params.key !== KEY_UP;
    if (noUpOrDownKeyPressed) {
      return suggestedNextCell;
    }

    gridOptions.api.forEachNode(function (node) {
      if (node.rowIndex === suggestedNextCell.rowIndex) {
        node.setSelected(true);
      }
    });

    return suggestedNextCell;
}
// End Keyboard up & down



// Select grid rowCell
function onSelectionChanged() {
    var selectedRows = gridOptions.api.getSelectedRows();
    document.querySelector('#selectedRows').innerHTML =
        selectedRows.length === 1 ? selectedRows[0].athlete : '';
}


var gridOptions = {
    columnDefs: columnDefs,
    defaultColDef: {
        sortable: true, // 정렬
        lockPinned: true, // 열고정
        lockPosition: true, // 열고정 + 첫번째 위치고정
        resizable: true, // 요소크기 재조정 여부 및 방법 지정
    },
    getRowNodeId: data => {
        return data.id;
    },
    immutableData: true,
    rowSelection: 'single',

    rowData: [], // grid data
    // colResizeDefault: 'shift',
    navigateToNextCell: navigateToNextCell,
    onSelectionChanged: onSelectionChanged,

};


// setup the grid after the page has finished loading
document.addEventListener('DOMContentLoaded', function () {
    var gridDiv = document.querySelector('#myGrid');
    new agGrid.Grid(gridDiv, gridOptions);
    gridOptions.api.setRowData(vulInfo);
});

