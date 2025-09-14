from django.urls import path
from . import views

urlpatterns = [
    path('members/', views.create_member, name='create_member'),
    path('members', views.create_member, name='create_member_no_slash'),
]
