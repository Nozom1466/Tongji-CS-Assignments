from django.urls import path
from . import views

urlpatterns = [
    path('toadmin/', views.toAdmin_view),
    path('admin/', views.admin_view),
    path('', views.toLogin_view),
    path('login/', views.Login_view),
    path('main/', views.main_view),
    path('toregister/', views.toregister_view),
    path('register/', views.register_view),
    path('uploaded/', views.upload_video),
    path('deleteItems/', views.deleteItems),
    path('deleted/', views.deleted),
    path('downItems/', views.downItems),
]