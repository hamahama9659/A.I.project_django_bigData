from django.urls import path
from . import views

app_name = 'detect'
urlpatterns = [

    # 로딩 화면
    path('',views.index, name='index'),

    # uploadview
    path('uploadview/', views.uploadview, name='uploadview'),

    # main list 화면
    path('listpage/', views.listpage, name='listpage'),

    # file upload
    path('upload/', views.upload, name='upload'),

    # delete file when close modal without predict
    path('modalclose/', views.modalclose, name='modalclose'),

    # insert basic data for list
    path('beforepredict/', views.beforepredict, name='beforepredict'),

    # start predicting
    path('startpredict/', views.startpredict, name='startpredict'),



]