from django.contrib import admin
from .models import Filename, Code, Vulnerability



# Filename Table
@admin.register(Filename)
class FilenameAdmin(admin.ModelAdmin):
    list_display = ('id', 'filename', 'regdate')
    list_display_links = ('filename',)
    search_fields = ('filename',)
    list_per_page = 20
    list_filter = ('filename','regdate') # 정렬 필터용 목록


# Code Table
@admin.register(Code)
class CodeAdmin(admin.ModelAdmin):
    list_display = ('fileid', 'code')
    list_display_links = ('fileid',)
    search_fields = ('fileid',)
    list_per_page = 20
    list_filter = ('fileid',) # 정렬 필터용 목록


# Vulnerability Table
@admin.register(Vulnerability)
class VulnerabilityAdmin(admin.ModelAdmin):
    list_display = ('id', 'filenameid', 'operatetime', 'endtime') # 취약점 5가지는 제외시킴
    list_display_links = ('filenameid', 'id',)
    search_fields = ('filenameid',)
    list_per_page = 20
    list_filter = ('filenameid','operatetime','endtime',) # 정렬 필터용 목록
