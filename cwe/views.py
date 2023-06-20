from django.shortcuts import render


#--- informations view
def info(request):
    if request.method == 'GET':
        return render(request, 'cwe/info.html')
