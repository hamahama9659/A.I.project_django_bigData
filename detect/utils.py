from uuid import uuid4
from django.utils import timezone
import os, re
from .models import Vulnerability, Filename, Code
from django.http import JsonResponse  #--- AJAX
import json  #--- dumps to json
from datetime import datetime

# --- get list data by request method
def calllistdata(request):
    vuls = Vulnerability.objects.select_related('filenameid').all()

    # casting QuerySet to Dictionary type
    vulInfo = []
    for vul in vuls:
        if vul.endtime == None :
            endtime = ''
        else :
            endtime = vul.endtime.strftime('%Y-%m-%d, %H:%M:%S')

        dic = {
            "operatetime": vul.operatetime.strftime('%Y-%m-%d, %H:%M:%S'),
            "endtime": endtime,
            "cwe119": vul.cwe119_float(),
            "cwe120": vul.cwe120_float(),
            "cwe469": vul.cwe469_float(),
            "cwe476": vul.cwe476_float(),
            "cweother": vul.cweother_float(),
            "filename": vul.filenameid.__str__(),
        }
        vulInfo.append(dic)  # append 'dic' to list type

    if request.method == 'GET':
        # dumping data to json
        context = {"vuls_js": json.dumps(vulInfo)}
        return context

    if request.method == 'POST':
        # return to JSONResponse
        jres = JsonResponse({"vuls_js": json.dumps(vulInfo)})
        return jres


#--- views.upload()
#--- 파일이름 중복 방지
def uuid_name(filename):
    ymd_path = timezone.now().strftime('%Y%m%d_')
    uuid_name = uuid4().hex # 32 characters <-> uuid4 = 36 characters
    origin_name = os.path.splitext(filename)[0] # 확장자를 뺀 파일이름 추출
    extension = os.path.splitext(filename)[-1].lower() # 확장자 추출 뒤 소문자로 변환
    return origin_name + '_' + ymd_path + uuid_name + extension

#--- source code 추출
def reading_source(path):
    try :
        # utf-8 인코딩용
        sourcecode = open(path,"r", encoding='utf-8')
        data = sourcecode.readlines()
    except :
        # utf-8 인코딩 오류시 cp949로 인코딩
        sourcecode = open(path,"r")
        data = sourcecode.readlines()
    finally:
        codes = ""
        for i in data:
            if i != None :
                # 스트링 값 추출을 위한 큰따옴표 처리
                functionName = re.sub('\"', '\\"',i)
                codes += functionName

    return codes


#--- Marchine Learning Model
import keras
from transformers import BertTokenizer
import numpy as np
import warnings
def predict_model(modelpath, sourcecode):
    warnings.filterwarnings('ignore')

    input_len = 1000
    tempsrc = sourcecode

    tokenizer = BertTokenizer.from_pretrained('bert-base-uncased')
    token = tokenizer.encode(tempsrc, max_length=input_len, truncation=True, padding='max_length')
    tokens = []
    tokens.append(token)
    datax = np.array(tokens)

    conv1dBTmodel = keras.models.load_model(modelpath)
    predicted = conv1dBTmodel.predict(datax)
    result = float(predicted[0])
    return result