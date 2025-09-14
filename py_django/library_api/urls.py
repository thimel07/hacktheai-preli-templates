"""
URL configuration for library_api project.
"""
from django.urls import path, include

urlpatterns = [
    path('api/', include('members.urls')),
]
