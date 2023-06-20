from django.urls import path
from . import views

app_name = 'cwe'
urlpatterns = [

    # informations 화면
    path('info/',views.info, name='info'),

]