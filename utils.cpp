inline void
Utils_Swap(int32* a, int32* b)
{
    int32 t = *a;
    *a = *b;
    *b = t;
}

inline
u32 Utils_StringLength(char* String)
{
    u32 Length = 0;
    while(*String++ != '\0') Length++;
    return Length;
}

inline void
Utils_CopyString(char* Source, char* Destination)
{
    char c = *Source++;
    while(c != '\0')
    {
        *Destination++ = c;
        c = *Source++;
    }
}

inline void
Utils_ConcatenateStrings(char* First, char* Second, char* Buffer, u32 BufferSize)
{
    u32 FirstStringLength = Utils_StringLength(First);
    u32 SecondStringLength = Utils_StringLength(Second);
    
    u32 TotalLength = 0;
    
    for(int32 i = 0; i < FirstStringLength; i++)
    {
        if(TotalLength >= BufferSize - 1) break;
        Buffer[TotalLength++] = *(First + i);
    }
    for(int32 i = 0; i < SecondStringLength; i++)
    {
        if(TotalLength >= BufferSize - 1) break;
        Buffer[TotalLength++] = *(Second + i);
    }
    Buffer[TotalLength] = 0;
}

u32
Utils_HashFNV1a(const void* data, size_t size, uint32_t seed = 0x811C9DC5u)
{
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    uint32_t hash = seed;
    for (size_t i = 0; i < size; ++i)
    {
        hash ^= bytes[i];
        hash *= 0x01000193u; // FNV prime
    }
    return hash;
}

// Helper for strings
u32
Utils_HashStringFNV1a(const char* str, uint32_t seed = 0x811C9DC5u)
{
    uint32_t hash = seed;
    while (*str)
    {
        hash ^= static_cast<uint8_t>(*str++);
        hash *= 0x01000193u;
    }
    return hash;
}