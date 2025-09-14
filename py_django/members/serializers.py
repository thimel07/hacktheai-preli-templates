from rest_framework import serializers


class MemberSerializer(serializers.Serializer):
    member_id = serializers.IntegerField()
    name = serializers.CharField(max_length=100)
    age = serializers.IntegerField(min_value=0, max_value=150)
    has_borrowed = serializers.BooleanField(read_only=True, default=False)
