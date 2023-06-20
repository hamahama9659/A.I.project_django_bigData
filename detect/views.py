from django.shortcuts import render
from .models import Vulnerability, Filename, Code
from django.http import JsonResponse  #--- AJAX
from .utils import calllistdata, uuid_name, reading_source, predict_model
from django.utils import timezone
import os

#--- index
def index(request):
    return render(request, 'detect/loading.html')


#--- uploadviwew
def uploadview(request):
    if request.method == 'GET':
        return render(request, 'detect/uploadView.html')


#--- list page
def listpage(request):
    if request.method == 'GET':
        context = calllistdata(request)
        return render(request, 'detect/list.html', context)


#--- file upload
def upload(request):
    # from app.js, formData
    file = request.FILES['file'].read()
    fileName= request.POST['filename']
    existingPath = request.POST['existingPath']
    end = request.POST['end']
    nextSlice = request.POST['nextSlice']

    uuid_filename = uuid_name(fileName) # 파일이름 암호화 및 중복방지

    if file=="" or fileName=="" or existingPath=="" or end=="" or nextSlice=="":
        fileInfo = ''
        return fileInfo
    else:
        # media 디렉토리에 파일저장
        path = 'media/' + uuid_filename
        with open(path, 'wb+') as destination:
            destination.write(file)

        # insert to Filename table
        FileFolder = Filename()
        FileFolder.existingPath = uuid_filename
        FileFolder.eof = end
        FileFolder.filename = fileName
        FileFolder.save()

        # insert to Code table
        CodeFolder = Code()
        CodeFolder.fileid = FileFolder
        CodeFolder.code = reading_source(path) # 소스코드 추출
        CodeFolder.save()

        fileid = FileFolder.id

        if int(end):
            res = JsonResponse({'data':'Uploaded Successfully'
                                   , 'existingPath': uuid_filename
                                   , 'fileid': fileid})
        else:
            res = JsonResponse({'existingPath': uuid_filename}) # 중복이름 방지

        return res


#--- delete uploaded file data
def modalclose(request):
    if request.method == 'POST':
        fileid = request.POST['fileid']
        fileData = Filename.objects.get(id=fileid)

        filePath = 'media/' + fileData.existingPath
        if os.path.isfile(filePath):
            os.remove(filePath)

        fileData.delete()

        res = JsonResponse({'data':'Cancel uploading file'})
        return res


#--- insert basic vulnerability data & reload list data
def beforepredict(request):
    if request.method == 'POST':
        fileid = request.POST['fileid']

        if fileid != '':
            FilenameInstance = Filename.objects.get(id=fileid)
            VulFolder = Vulnerability(filenameid=FilenameInstance)
            VulFolder.endtime = None

            #--- 1차 저장
            VulFolder.save()

        jres = calllistdata(request)
        return jres


#--- start predictor
def startpredict(request):
    if request.method == 'POST':
        fileid = request.POST['fileid']

        if fileid != '':
            #--- 분석모델 작동 및 결과 추출
            file = Code.objects.select_related('fileid').get(fileid = fileid)
            cwe119 = predict_model('models/CWE-119.h5', file.code.__str__())
            cwe120 = predict_model('models/CWE-120.h5', file.code.__str__())
            cwe469 = predict_model('models/CWE-469.h5', file.code.__str__())
            cwe476 = predict_model('models/CWE-476.h5', file.code.__str__())
            cweother = predict_model('models/CWE-other.h5', file.code.__str__())

            #--- 분석결과 insert DB
            VulFolder = Vulnerability.objects.get(filenameid=fileid)
            VulFolder.cwe119 = cwe119
            VulFolder.cwe120 = cwe120
            VulFolder.cwe469 = cwe469
            VulFolder.cwe476 = cwe476
            VulFolder.cweother = cweother
            VulFolder.endtime = timezone.now()
            VulFolder.save()

        jres = calllistdata(request)
        return jres






