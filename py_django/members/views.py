from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.response import Response
from .models import members
from .serializers import MemberSerializer


@api_view(['POST'])
def create_member(request):
    """Create a new member in the library system"""
    serializer = MemberSerializer(data=request.data)
    
    if serializer.is_valid():
        # Check for duplicate member_id
        member_id = serializer.validated_data['member_id']
        if member_id in members:
            return Response(
                {"detail": f"member with id: {member_id} already exists"},
                status=status.HTTP_409_CONFLICT
            )
        
        # Create new member
        new_member = {
            "member_id": member_id,
            "name": serializer.validated_data['name'],
            "age": serializer.validated_data['age'],
            "has_borrowed": False
        }
        
        members[member_id] = new_member
        return Response(new_member, status=status.HTTP_201_CREATED)
    
    return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

