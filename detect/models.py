from django.db import models


# filename 테이블
class Filename(models.Model):
    class Meta:
        db_table = "filename"
        ordering = ('-regdate',) # 정렬 : 'regdate' 내림차순 (=최신순)

    existingPath = models.CharField(null=True, unique=True, max_length=100)
    filename = models.CharField(max_length=1024)
    regdate = models.DateTimeField(auto_now_add=True)
    eof = models.BooleanField(null=True)

    def __str__(self):
        return self.filename



# code 테이블
class Code(models.Model):
    class Meta:
        db_table = "code"

    # 1대1 관계 -> OneToOneFeild(Filename,...)
    # 식별 관계 -> 'primary_key=True'
    fileid = models.OneToOneField(Filename, verbose_name='fileid', primary_key=True, on_delete=models.CASCADE)
    code = models.TextField()

    def __str__(self):
        return self.code


# vulnerability 테이블
class Vulnerability(models.Model):
    class Meta:
        db_table = "vulnerability"
        ordering = ('-operatetime',) # 정렬 : 'operatetime' 내림차순 (=최신순)

    operatetime = models.DateTimeField(auto_now_add=True)
    endtime = models.DateTimeField(null=True, blank=True)

    # 5가지 취약점종류 데이터 형식 : 숫자, 총 11자리 중 소수점은 5자리 (정수는 (11-5=6) 6자리)
    cwe119 = models.DecimalField(max_digits = 11, decimal_places = 5, null=True, default=2)
    cwe120 = models.DecimalField(max_digits = 11, decimal_places = 5, null=True, default=2)
    cwe469 = models.DecimalField(max_digits = 11, decimal_places = 5, null=True, default=2)
    cwe476 = models.DecimalField(max_digits = 11, decimal_places = 5, null=True, default=2)
    cweother = models.DecimalField(max_digits = 11, decimal_places = 5, null=True, default=2)

    # 1대1 관계 -> OneToOneFeild 사용 (filename과 비식별 관계)
    filenameid = models.OneToOneField(Filename, verbose_name='filenameid', on_delete=models.CASCADE)

    def cwe119_float(self):
        return float(self.cwe119)
    def cwe120_float(self):
        return float(self.cwe120)
    def cwe469_float(self):
        return float(self.cwe469)
    def cwe476_float(self):
        return float(self.cwe476)
    def cweother_float(self):
        return float(self.cweother)












