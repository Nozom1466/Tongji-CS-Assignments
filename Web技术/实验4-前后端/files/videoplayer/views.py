import json

from django.conf import settings
from django.http import HttpResponse, JsonResponse, HttpResponseBadRequest, FileResponse
from django.views.decorators.csrf import csrf_exempt
from django.shortcuts import render, get_object_or_404
from .models import *
import random
import os


# Create your views here.

def toAdmin_view(request):
    return render(request, 'admin.html')


def admin_view(request):
    videos = VideoName.objects.all()
    data = list(videos.values())
    rt = {'data': json.dumps(data)}
    return render(request, 'manage.html', rt)


def toLogin_view(request):
    return render(request, 'login.html')


def Login_view(request):
    u = request.POST.get("user", '')
    p = request.POST.get("pwd", '')

    videos = VideoName.objects.all()
    data = list(videos.values())
    rt = {'data': json.dumps(data)}

    if u and p:
        c = VideoplayerStudentinfo.objects.filter(stu_name=u, stu_psw=p).count()
        if c >= 1:
            return render(request, 'main.html', rt)
        else:
            return HttpResponse("账号密码错误！")
    else:
        return HttpResponse("请输入正确账号和密码!")


def main_view(request):
    videos = VideoName.objects.all()
    data = list(videos.values())
    rt = {'data': json.dumps(data)}
    # print(rt)
    return render(request, 'main.html', rt)


def toregister_view(request):
    return render(request, "register.html")


def register_view(request):
    u = request.POST.get("user", '')
    p = request.POST.get("pwd", '')

    videos = VideoName.objects.all()
    data = list(videos.values())
    rt = {'data': json.dumps(data)}

    if u and p:
        stu = VideoplayerStudentinfo(stu_id=random.randrange(111111, 999999), stu_name=u, stu_psw=p)
        print(stu)
        stu.save()
        return render(request, 'main.html', rt)
    else:
        return HttpResponse("请输入完整的账号和密码")


def upload_video(request):
    print(11111)
    if request.method == 'POST':

        video_file = request.FILES['video_file']
        video_data = video_file.read()

        rename = request.POST.get("fileRename", '')

        n = len(os.listdir(r"D:\Web作业\实验4-前后端\videoplayer\videoplayer\static\videos"))

        video_uploaded = VideoName(video_id=n + 1, video_name=rename)
        video_uploaded.save()

        new_filename = "video" + str(n + 1) + ".mp4"

        dir_path = r'D:\Web作业\实验4-前后端\videoplayer\videoplayer\static\videos'

        with open(os.path.join(dir_path, new_filename), 'wb') as f:
            f.write(video_data)
        return render(request, "uploaded.html")
    else:
        return HttpResponse("Failed")


def deleteItems(request):
    if request.method == 'POST':
        items_to_delete = request.POST.getlist('itemsToDelete[]')
        for item_id in items_to_delete:
            # 获取指定的记录并删除
            item = get_object_or_404(VideoName, video_id=item_id)
            item.delete()
        success = True
        data = {'message': "SUCCESS"}
    else:
        success = False
        data = {'message': "FAIL"}
    return JsonResponse(data, status=200 if success else 400)


def deleted(request):
    return render(request, "deleted.html")


def downItems(request):
    if request.method == 'POST':
        items_to_download = request.POST.getlist('itemsToDownload[]')
        # print(int(items_to_download[0]))
        sq = items_to_download[0]
        file_name = r"video" + sq + ".mp4"
        # file_path = r"../static/videos/" + file_name
        file_path = os.path.join(settings.BASE_DIR, 'videoplayer\\static\\videos\\'+file_name)
        # D:\Web作业\实验4-前后端\videoplayer\videoplayer\static\videos
        video_file = open(file_path, 'rb')
        response = FileResponse(video_file)
        response['Content-Disposition'] = 'attachment;filename="%s"' % file_name
    else:
        response = {"message": "OK"}
    print(response)
    return response

