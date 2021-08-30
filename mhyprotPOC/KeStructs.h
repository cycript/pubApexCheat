#ifndef _SUPERFETCH_NATIVE_H
#define _SUPERFETCH_NATIVE_H
#pragma comment(lib, "ntdll.lib")
#define _AMD64_
#include <minwindef.h>
#include <Windows.h>

typedef long NTSTATUS, * PNTSTATUS;
#define NT_SUCCESS(Status)              (((NTSTATUS)(Status)) >= 0)
#include <fstream>
#include <cassert>
#include <functional>

#define WOW64_POINTER(Type) ULONG

typedef struct _STRING32 {
	USHORT   Length;
	USHORT   MaximumLength;
	ULONG  Buffer;
} STRING32, * PSTRING32,
UNICODE_STRING32, * PUNICODE_STRING32,
ANSI_STRING32, * PANSI_STRING32;

typedef struct _LDR_DATA_TABLE_ENTRY32
{
	LIST_ENTRY32 InLoadOrderLinks;
	LIST_ENTRY32 InMemoryOrderLinks;
	union
	{
		LIST_ENTRY32 InInitializationOrderLinks;
		LIST_ENTRY32 InProgressLinks;
	};
	WOW64_POINTER(PVOID) DllBase;
	WOW64_POINTER(PVOID) EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING32 FullDllName;
	UNICODE_STRING32 BaseDllName;
	union
	{
		UCHAR FlagGroup[4];
		ULONG Flags;
		struct
		{
			ULONG PackagedBinary : 1;
			ULONG MarkedForRemoval : 1;
			ULONG ImageDll : 1;
			ULONG LoadNotificationsSent : 1;
			ULONG TelemetryEntryProcessed : 1;
			ULONG ProcessStaticImport : 1;
			ULONG InLegacyLists : 1;
			ULONG InIndexes : 1;
			ULONG ShimDll : 1;
			ULONG InExceptionTable : 1;
			ULONG ReservedFlags1 : 2;
			ULONG LoadInProgress : 1;
			ULONG LoadConfigProcessed : 1;
			ULONG EntryProcessed : 1;
			ULONG ProtectDelayLoad : 1;
			ULONG ReservedFlags3 : 2;
			ULONG DontCallForThreads : 1;
			ULONG ProcessAttachCalled : 1;
			ULONG ProcessAttachFailed : 1;
			ULONG CorDeferredValidate : 1;
			ULONG CorImage : 1;
			ULONG DontRelocate : 1;
			ULONG CorILOnly : 1;
			ULONG ChpeImage : 1;
			ULONG ReservedFlags5 : 2;
			ULONG Redirected : 1;
			ULONG ReservedFlags6 : 2;
			ULONG CompatDatabaseProcessed : 1;
		};
	};
	USHORT ObsoleteLoadCount;
	USHORT TlsIndex;
	LIST_ENTRY32 HashLinks;
	ULONG TimeDateStamp;
	WOW64_POINTER(struct _ACTIVATION_CONTEXT*) EntryPointActivationContext;
	WOW64_POINTER(PVOID) Lock;
	WOW64_POINTER(PLDR_DDAG_NODE) DdagNode;
	LIST_ENTRY32 NodeModuleLink;
	WOW64_POINTER(struct _LDRP_LOAD_CONTEXT*) LoadContext;
	WOW64_POINTER(PVOID) ParentDllBase;
	WOW64_POINTER(PVOID) SwitchBackContext;
	//RTL_BALANCED_NODE32 BaseAddressIndexNode;
	//RTL_BALANCED_NODE32 MappingInfoIndexNode;
	WOW64_POINTER(ULONG_PTR) OriginalBase;
	LARGE_INTEGER LoadTime;
	ULONG BaseNameHashValue;
	//LDR_DLL_LOAD_REASON LoadReason;
	ULONG ImplicitPathOptions;
	ULONG ReferenceCount;
	ULONG DependentLoadFlags;
	UCHAR SigningLevel; // since REDSTONE2
} LDR_DATA_TABLE_ENTRY32, * PLDR_DATA_TABLE_ENTRY32;

typedef struct _PEB_LDR_DATA32
{
	ULONG Length;
	UCHAR Initialized;
	ULONG SsHandle;
	LIST_ENTRY32 InLoadOrderModuleList;
	LIST_ENTRY32 InMemoryOrderModuleList;
	LIST_ENTRY32 InInitializationOrderModuleList;
} PEB_LDR_DATA32, * PPEB_LDR_DATA32;

typedef enum _SUPERFETCH_INFORMATION_CLASS {
	SuperfetchRetrieveTrace = 1,        // Query
	SuperfetchSystemParameters = 2,     // Query
	SuperfetchLogEvent = 3,             // Set
	SuperfetchGenerateTrace = 4,        // Set
	SuperfetchPrefetch = 5,             // Set
	SuperfetchPfnQuery = 6,             // Query
	SuperfetchPfnSetPriority = 7,       // Set
	SuperfetchPrivSourceQuery = 8,      // Query
	SuperfetchSequenceNumberQuery = 9,  // Query
	SuperfetchScenarioPhase = 10,       // Set
	SuperfetchWorkerPriority = 11,      // Set
	SuperfetchScenarioQuery = 12,       // Query
	SuperfetchScenarioPrefetch = 13,    // Set
	SuperfetchRobustnessControl = 14,   // Set
	SuperfetchTimeControl = 15,         // Set
	SuperfetchMemoryListQuery = 16,     // Query
	SuperfetchMemoryRangesQuery = 17,   // Query
	SuperfetchTracingControl = 18,       // Set
	SuperfetchTrimWhileAgingControl = 19,
	SuperfetchInformationMax = 20
} SUPERFETCH_INFORMATION_CLASS;

//
// Buffer for NtQuery/SetInformationSystem for the Superfetch Class
//
typedef struct _SUPERFETCH_INFORMATION {
	ULONG Version;
	ULONG Magic;
	SUPERFETCH_INFORMATION_CLASS InfoClass;
	PVOID Data;
	ULONG Length;
} SUPERFETCH_INFORMATION, * PSUPERFETCH_INFORMATION;

typedef struct _RTL_BITMAP {
	ULONG SizeOfBitMap;
	PULONG Buffer;
} RTL_BITMAP, * PRTL_BITMAP;

typedef struct _PF_PHYSICAL_MEMORY_RANGE {
	ULONG_PTR BasePfn;
	ULONG_PTR PageCount;
} PF_PHYSICAL_MEMORY_RANGE, * PPF_PHYSICAL_MEMORY_RANGE;

typedef struct _PF_MEMORY_RANGE_INFO {
	ULONG Version;
	ULONG RangeCount;
	PF_PHYSICAL_MEMORY_RANGE Ranges[ANYSIZE_ARRAY];
} PF_MEMORY_RANGE_INFO, * PPF_MEMORY_RANGE_INFO;

typedef struct _PHYSICAL_MEMORY_RUN {
	SIZE_T BasePage;
	SIZE_T PageCount;
} PHYSICAL_MEMORY_RUN, * PPHYSICAL_MEMORY_RUN;


typedef struct _SYSTEM_BASIC_INFORMATION {
	ULONG Reserved;
	ULONG TimerResolution;
	ULONG PageSize;
	ULONG NumberOfPhysicalPages;
	ULONG LowestPhysicalPageNumber;
	ULONG HighestPhysicalPageNumber;
	ULONG AllocationGranularity;
	ULONG_PTR MinimumUserModeAddress;
	ULONG_PTR MaximumUserModeAddress;
	ULONG_PTR ActiveProcessorsAffinityMask;
	CCHAR NumberOfProcessors;
} SYSTEM_BASIC_INFORMATION, * PSYSTEM_BASIC_INFORMATION;

struct RTL_PROCESS_MODULE_INFORMATION
{
	unsigned int Section;
	void* MappedBase;
	void* ImageBase;
	unsigned int ImageSize;
	unsigned int Flags;
	unsigned short LoadOrderIndex;
	unsigned short InitOrderIndex;
	unsigned short LoadCount;
	unsigned short OffsetToFileName;
	char FullPathName[256];
};

typedef struct RTL_PROCESS_MODULES
{
	unsigned int NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[0];
}RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

struct SYSTEM_HANDLE
{
	ULONG ProcessId;
	BYTE ObjectTypeNumber;
	BYTE Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
};

struct SYSTEM_HANDLE_INFORMATION
{
	ULONG HandleCount;
	SYSTEM_HANDLE Handles[0];
};

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,
	SystemProcessorInformation,
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPageFileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemFileCacheInformation,
	SystemPoolTagInformation,
	SystemInterruptInformation,
	SystemDpcBehaviorInformation,
	SystemFullMemoryInformation,
	SystemLoadGdiDriverInformation,
	SystemUnloadGdiDriverInformation,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemNextEventIdInformation,
	SystemEventIdsInformation,
	SystemCrashDumpInformation,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemExtendServiceTableInformation,
	SystemPrioritySeperation,
	SystemPlugPlayBusInformation,
	SystemDockInformation,
	//SystemPowerInformation,
	SystemProcessorSpeedInformation,
	SystemCurrentTimeZoneInformation,
	SystemLookasideInformation


} SYSTEM_INFORMATION_CLASS, * PSYSTEM_INFORMATION_CLASS;

extern "C" NTSTATUS WINAPI NtQuerySystemInformation(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
);


#define PAGE_SHIFT 12
#define PAGE_SIZE (1 << 12)

#define SE_DEBUG_PRIVILEGE                (20L)
#define SE_PROF_SINGLE_PROCESS_PRIVILEGE  (13L)

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(
	IN ULONG Privilege,
	IN BOOLEAN NewValue,
	IN BOOLEAN ForThread,
	OUT PBOOLEAN OldValue
);

#define SPAGE_SIZE              0x1000
#define SUPERFETCH_VERSION      45
#define SUPERFETCH_MAGIC        'kuhC'

#endif

#define PAGE_SHIFT 12
#define PAGE_SIZE (1 << 12)

#define SE_DEBUG_PRIVILEGE                (20L)
#define SE_PROF_SINGLE_PROCESS_PRIVILEGE  (13L)

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(
	IN ULONG Privilege,
	IN BOOLEAN NewValue,
	IN BOOLEAN ForThread,
	OUT PBOOLEAN OldValue
);

#define SPAGE_SIZE              0x1000
#define SUPERFETCH_VERSION      45
#define SUPERFETCH_MAGIC        'kuhC'

#define FIELD_OFFSET(type, field)    ((LONG)(LONG_PTR)&(((type *)0)->field))
#define MI_GET_PFN(x)                   (PMMPFN_IDENTITY)(&MmPfnDatabase->PageData[(x)])

typedef struct _SYSTEM_MEMORY_LIST_INFORMATION {
	SIZE_T ZeroPageCount;
	SIZE_T FreePageCount;
	SIZE_T ModifiedPageCount;
	SIZE_T ModifiedNoWritePageCount;
	SIZE_T BadPageCount;
	SIZE_T PageCountByPriority[8];
	SIZE_T RepurposedPagesByPriority[8];
	ULONG_PTR ModifiedPageCountPageFile;
} SYSTEM_MEMORY_LIST_INFORMATION, * PSYSTEM_MEMORY_LIST_INFO;

typedef struct _MEMORY_FRAME_INFORMATION {
	ULONGLONG UseDescription : 4;
	ULONGLONG ListDescription : 3;
	ULONGLONG Reserved0 : 1;
	ULONGLONG Pinned : 1;
	ULONGLONG DontUse : 48;
	ULONGLONG Priority : 3;
	ULONGLONG Reserved : 4;
} MEMORY_FRAME_INFORMATION, * PMEMORY_FRAME_INFORMATION;

typedef struct _FILEOFFSET_INFORMATION {
	ULONGLONG DontUse : 9;
	ULONGLONG Offset : 48;
	ULONGLONG Reserved : 7;
} FILEOFFSET_INFORMATION, * PFILEOFFSET_INFORMATION;

typedef struct _PAGEDIR_INFORMATION {
	ULONGLONG DontUse : 9;
	ULONGLONG PageDirectoryBase : 48;
	ULONGLONG Reserved : 7;
} PAGEDIR_INFORMATION, * PPAGEDIR_INFORMATION;

typedef struct _UNIQUE_PROCESS_INFORMATION {
	ULONGLONG DontUse : 9;
	ULONGLONG UniqueProcessKey : 48;
	ULONGLONG Reserved : 7;
} UNIQUE_PROCESS_INFORMATION, * PUNIQUE_PROCESS_INFORMATION;

typedef struct _MMPFN_IDENTITY {
	union {
		MEMORY_FRAME_INFORMATION e1;
		FILEOFFSET_INFORMATION e2;
		PAGEDIR_INFORMATION e3;
		UNIQUE_PROCESS_INFORMATION e4;
	} u1;
	SIZE_T PageFrameIndex;
	union {
		struct {
			ULONG Image : 1;
			ULONG Mismatch : 1;
		} e1;
		PVOID FileObject;
		PVOID UniqueFileObjectKey;
		PVOID ProtoPteAddress;
		PVOID VirtualAddress;
	} u2;
} MMPFN_IDENTITY, * PMMPFN_IDENTITY;

typedef struct _PF_PFN_PRIO_REQUEST {
	ULONG Version;
	ULONG RequestFlags;
	SIZE_T PfnCount;
	SYSTEM_MEMORY_LIST_INFORMATION MemInfo;
	MMPFN_IDENTITY PageData[256];
} PF_PFN_PRIO_REQUEST, * PPF_PFN_PRIO_REQUEST;

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif

#define STATUS_SUCCESS                   ((NTSTATUS)0x00000000L)
#define STATUS_UNSUCCESSFUL              ((NTSTATUS)0xC0000001L)
#define STATUS_NOT_IMPLEMENTED           ((NTSTATUS)0xC0000002L)
#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)
#define STATUS_INVALID_CID               ((NTSTATUS)0xC000000BL)
#define STATUS_NO_SUCH_DEVICE            ((NTSTATUS)0xC000000EL)
#define STATUS_NO_SUCH_FILE              ((NTSTATUS)0xC000000FL)
#define STATUS_INVALID_DEVICE_REQUEST    ((NTSTATUS)0xC0000010L)
#define STATUS_MORE_PROCESSING_REQUIRED  ((NTSTATUS)0xC0000016L)
#define STATUS_CONFLICTING_ADDRESSES     ((NTSTATUS)0xC0000018L)
#define STATUS_NO_MORE_ENTRIES           ((NTSTATUS)0x8000001AL)
#define STATUS_BUFFER_TOO_SMALL          ((NTSTATUS)0xC0000023L)
#define STATUS_INVALID_PAGE_PROTECTION   ((NTSTATUS)0xC0000045L)
#define STATUS_PROCEDURE_NOT_FOUND       ((NTSTATUS)0xC000007AL)
#define STATUS_INSUFFICIENT_RESOURCES    ((NTSTATUS)0xC000009AL)
#define STATUS_INSTRUCTION_MISALIGNMENT  ((NTSTATUS)0xC00000AAL)
#define STATUS_INTERNAL_ERROR            ((NTSTATUS)0xC00000E5L)
#define STATUS_INVALID_PARAMETER_1       ((NTSTATUS)0xC00000EFL)
#define STATUS_INVALID_PARAMETER_2       ((NTSTATUS)0xC00000F0L)
#define STATUS_INVALID_PARAMETER_3       ((NTSTATUS)0xC00000F1L)
#define STATUS_INVALID_PARAMETER_4       ((NTSTATUS)0xC00000F2L)
#define STATUS_INVALID_PARAMETER_5       ((NTSTATUS)0xC00000F3L)
#define STATUS_INVALID_PARAMETER_6       ((NTSTATUS)0xC00000F4L)
#define STATUS_INVALID_PARAMETER_7       ((NTSTATUS)0xC00000F5L)
#define STATUS_INVALID_PARAMETER_8       ((NTSTATUS)0xC00000F6L)
#define STATUS_INVALID_PARAMETER_9       ((NTSTATUS)0xC00000F7L)
#define STATUS_INVALID_PARAMETER_10      ((NTSTATUS)0xC00000F8L)
#define STATUS_INVALID_PARAMETER_11      ((NTSTATUS)0xC00000F9L)
#define STATUS_INVALID_PARAMETER_12      ((NTSTATUS)0xC00000FAL)
#define STATUS_INVALID_ADDRESS           ((NTSTATUS)0xC0000141L)
#define STATUS_DATATYPE_MISALIGNMENT_ERROR ((NTSTATUS)0xC00002C5L)

typedef long LONG;

typedef LONG KPRIORITY;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING* PUNICODE_STRING;
typedef const UNICODE_STRING* PCUNICODE_STRING;

struct SFMemoryInfo
{
	uint64_t Start;
	uint64_t End;
	int PageCount;
	uint64_t Size;
};
#define PHYSICAL_ADDRESS	LARGE_INTEGER

typedef struct _CURDIR
{
	UNICODE_STRING DosPath;
	PVOID Handle;
} CURDIR, * PCURDIR;

typedef struct _STRING
{
	WORD Length;
	WORD MaximumLength;
	CHAR* Buffer;
} STRING, * PSTRING;


typedef struct _RTL_DRIVE_LETTER_CURDIR
{
	WORD Flags;
	WORD Length;
	ULONG TimeStamp;
	STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, * PRTL_DRIVE_LETTER_CURDIR;


typedef struct _RTL_USER_PROCESS_PARAMETERS
{
	ULONG MaximumLength;
	ULONG Length;
	ULONG Flags;
	ULONG DebugFlags;
	PVOID ConsoleHandle;
	ULONG ConsoleFlags;
	PVOID StandardInput;
	PVOID StandardOutput;
	PVOID StandardError;
	CURDIR CurrentDirectory;
	UNICODE_STRING DllPath;
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
	PVOID Environment;
	ULONG StartingX;
	ULONG StartingY;
	ULONG CountX;
	ULONG CountY;
	ULONG CountCharsX;
	ULONG CountCharsY;
	ULONG FillAttribute;
	ULONG WindowFlags;
	ULONG ShowWindowFlags;
	UNICODE_STRING WindowTitle;
	UNICODE_STRING DesktopInfo;
	UNICODE_STRING ShellInfo;
	UNICODE_STRING RuntimeData;
	RTL_DRIVE_LETTER_CURDIR CurrentDirectores[32];
	ULONG EnvironmentSize;
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	WORD LoadCount;
	WORD TlsIndex;
	union
	{
		LIST_ENTRY HashLinks;
		struct
		{
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union
	{
		ULONG TimeDateStamp;
		PVOID LoadedImports;
	};
	_ACTIVATION_CONTEXT* EntryPointActivationContext;
	PVOID PatchInformation;
	LIST_ENTRY ForwarderLinks;
	LIST_ENTRY ServiceTagLinks;
	LIST_ENTRY StaticLinks;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB_LDR_DATA
{
	ULONG Length;
	UCHAR Initialized;
	PVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID EntryInProgress;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB32
{
	UCHAR InheritedAddressSpace;                                            //0x0
	UCHAR ReadImageFileExecOptions;                                         //0x1
	UCHAR BeingDebugged;                                                    //0x2
	union
	{
		UCHAR BitField;                                                     //0x3
		struct
		{
			UCHAR ImageUsesLargePages : 1;                                    //0x3
			UCHAR IsProtectedProcess : 1;                                     //0x3
			UCHAR IsImageDynamicallyRelocated : 1;                            //0x3
			UCHAR SkipPatchingUser32Forwarders : 1;                           //0x3
			UCHAR IsPackagedProcess : 1;                                      //0x3
			UCHAR IsAppContainer : 1;                                         //0x3
			UCHAR IsProtectedProcessLight : 1;                                //0x3
			UCHAR IsLongPathAwareProcess : 1;                                 //0x3
		};
	};
	ULONG Mutant;                                                           //0x4
	ULONG ImageBaseAddress;                                                 //0x8
	ULONG Ldr;                                                              //0xc
	ULONG ProcessParameters;                                                //0x10
	ULONG SubSystemData;                                                    //0x14
	ULONG ProcessHeap;                                                      //0x18
	ULONG FastPebLock;                                                      //0x1c
	ULONG AtlThunkSListPtr;                                                 //0x20
	ULONG IFEOKey;                                                          //0x24
	union
	{
		ULONG CrossProcessFlags;                                            //0x28
		struct
		{
			ULONG ProcessInJob : 1;                                           //0x28
			ULONG ProcessInitializing : 1;                                    //0x28
			ULONG ProcessUsingVEH : 1;                                        //0x28
			ULONG ProcessUsingVCH : 1;                                        //0x28
			ULONG ProcessUsingFTH : 1;                                        //0x28
			ULONG ProcessPreviouslyThrottled : 1;                             //0x28
			ULONG ProcessCurrentlyThrottled : 1;                              //0x28
			ULONG ProcessImagesHotPatched : 1;                                //0x28
			ULONG ReservedBits0 : 24;                                         //0x28
		};
	};
	union
	{
		ULONG KernelCallbackTable;                                          //0x2c
		ULONG UserSharedInfoPtr;                                            //0x2c
	};
	ULONG SystemReserved;                                                   //0x30
	ULONG AtlThunkSListPtr32;                                               //0x34
	ULONG ApiSetMap;                                                        //0x38
	ULONG TlsExpansionCounter;                                              //0x3c
	ULONG TlsBitmap;                                                        //0x40
	ULONG TlsBitmapBits[2];                                                 //0x44
	ULONG ReadOnlySharedMemoryBase;                                         //0x4c
	ULONG SharedData;                                                       //0x50
	ULONG ReadOnlyStaticServerData;                                         //0x54
	ULONG AnsiCodePageData;                                                 //0x58
	ULONG OemCodePageData;                                                  //0x5c
	ULONG UnicodeCaseTableData;                                             //0x60
	ULONG NumberOfProcessors;                                               //0x64
	ULONG NtGlobalFlag;                                                     //0x68
	union _LARGE_INTEGER CriticalSectionTimeout;                            //0x70
	ULONG HeapSegmentReserve;                                               //0x78
	ULONG HeapSegmentCommit;                                                //0x7c
	ULONG HeapDeCommitTotalFreeThreshold;                                   //0x80
	ULONG HeapDeCommitFreeBlockThreshold;                                   //0x84
	ULONG NumberOfHeaps;                                                    //0x88
	ULONG MaximumNumberOfHeaps;                                             //0x8c
	ULONG ProcessHeaps;                                                     //0x90
	ULONG GdiSharedHandleTable;                                             //0x94
	ULONG ProcessStarterHelper;                                             //0x98
	ULONG GdiDCAttributeList;                                               //0x9c
	ULONG LoaderLock;                                                       //0xa0
	ULONG OSMajorVersion;                                                   //0xa4
	ULONG OSMinorVersion;                                                   //0xa8
	USHORT OSBuildNumber;                                                   //0xac
	USHORT OSCSDVersion;                                                    //0xae
	ULONG OSPlatformId;                                                     //0xb0
	ULONG ImageSubsystem;                                                   //0xb4
	ULONG ImageSubsystemMajorVersion;                                       //0xb8
	ULONG ImageSubsystemMinorVersion;                                       //0xbc
	ULONG ActiveProcessAffinityMask;                                        //0xc0
	ULONG GdiHandleBuffer[34];                                              //0xc4
	ULONG PostProcessInitRoutine;                                           //0x14c
	ULONG TlsExpansionBitmap;                                               //0x150
	ULONG TlsExpansionBitmapBits[32];                                       //0x154
	ULONG SessionId;                                                        //0x1d4
	union _ULARGE_INTEGER AppCompatFlags;                                   //0x1d8
	union _ULARGE_INTEGER AppCompatFlagsUser;                               //0x1e0
	ULONG pShimData;                                                        //0x1e8
	ULONG AppCompatInfo;                                                    //0x1ec
	struct _STRING32 CSDVersion;                                            //0x1f0
	ULONG ActivationContextData;                                            //0x1f8
	ULONG ProcessAssemblyStorageMap;                                        //0x1fc
	ULONG SystemDefaultActivationContextData;                               //0x200
	ULONG SystemAssemblyStorageMap;                                         //0x204
	ULONG MinimumStackCommit;                                               //0x208
	ULONG SparePointers[4];                                                 //0x20c
	ULONG SpareUlongs[5];                                                   //0x21c
	ULONG WerRegistrationData;                                              //0x230
	ULONG WerShipAssertPtr;                                                 //0x234
	ULONG pUnused;                                                          //0x238
	ULONG pImageHeaderHash;                                                 //0x23c
	union
	{
		ULONG TracingFlags;                                                 //0x240
		struct
		{
			ULONG HeapTracingEnabled : 1;                                     //0x240
			ULONG CritSecTracingEnabled : 1;                                  //0x240
			ULONG LibLoaderTracingEnabled : 1;                                //0x240
			ULONG SpareTracingBits : 29;                                      //0x240
		};
	};
	ULONGLONG CsrServerReadOnlySharedMemoryBase;                            //0x248
	ULONG TppWorkerpListLock;                                               //0x250
	struct LIST_ENTRY32 TppWorkerpList;                                     //0x254
	ULONG WaitOnAddressHashTable[128];                                      //0x25c
	ULONG TelemetryCoverageHeader;                                          //0x45c
	ULONG CloudFileFlags;                                                   //0x460
	ULONG CloudFileDiagFlags;                                               //0x464
	CHAR PlaceholderCompatibilityMode;                                      //0x468
	CHAR PlaceholderCompatibilityModeReserved[7];                           //0x469
	ULONG LeapSecondData;                                                   //0x470
	union
	{
		ULONG LeapSecondFlags;                                              //0x474
		struct
		{
			ULONG SixtySecondEnabled : 1;                                     //0x474
			ULONG Reserved : 31;                                              //0x474
		};
	};
	ULONG NtGlobalFlag2;                                                    //0x478
}PEB32, *PPEB32;

typedef struct _PEB
{
	UCHAR InheritedAddressSpace;
	UCHAR ReadImageFileExecOptions;
	UCHAR BeingDebugged;
	UCHAR BitField;
	ULONG ImageUsesLargePages : 1;
	ULONG IsProtectedProcess : 1;
	ULONG IsLegacyProcess : 1;
	ULONG IsImageDynamicallyRelocated : 1;
	ULONG SpareBits : 4;
	PVOID Mutant;
	PVOID ImageBaseAddress;
	PPEB_LDR_DATA Ldr;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	PVOID SubSystemData;
	PVOID ProcessHeap;
	PRTL_CRITICAL_SECTION FastPebLock;
	PVOID AtlThunkSListPtr;
	PVOID IFEOKey;
	ULONG CrossProcessFlags;
	ULONG ProcessInJob : 1;
	ULONG ProcessInitializing : 1;
	ULONG ReservedBits0 : 30;
	union
	{
		PVOID KernelCallbackTable;
		PVOID UserSharedInfoPtr;
	};
	ULONG SystemReserved[1];
	ULONG SpareUlong;
	//PPEB_FREE_BLOCK FreeList;
	ULONG TlsExpansionCounter;
	PVOID TlsBitmap;
	ULONG TlsBitmapBits[2];
	PVOID ReadOnlySharedMemoryBase;
	PVOID HotpatchInformation;
	VOID** ReadOnlyStaticServerData;
	PVOID AnsiCodePageData;
	PVOID OemCodePageData;
	PVOID UnicodeCaseTableData;
	ULONG NumberOfProcessors;
	ULONG NtGlobalFlag;
	LARGE_INTEGER CriticalSectionTimeout;
	ULONG HeapSegmentReserve;
	ULONG HeapSegmentCommit;
	ULONG HeapDeCommitTotalFreeThreshold;
	ULONG HeapDeCommitFreeBlockThreshold;
	ULONG NumberOfHeaps;
	ULONG MaximumNumberOfHeaps;
	VOID** ProcessHeaps;
	PVOID GdiSharedHandleTable;
	PVOID ProcessStarterHelper;
	ULONG GdiDCAttributeList;
	PRTL_CRITICAL_SECTION LoaderLock;
	ULONG OSMajorVersion;
	ULONG OSMinorVersion;
	WORD OSBuildNumber;
	WORD OSCSDVersion;
	ULONG OSPlatformId;
	ULONG ImageSubsystem;
	ULONG ImageSubsystemMajorVersion;
	ULONG ImageSubsystemMinorVersion;
	ULONG ImageProcessAffinityMask;
	ULONG GdiHandleBuffer[34];
	PVOID PostProcessInitRoutine;
	PVOID TlsExpansionBitmap;
	ULONG TlsExpansionBitmapBits[32];
	ULONG SessionId;
	ULARGE_INTEGER AppCompatFlags;
	ULARGE_INTEGER AppCompatFlagsUser;
	PVOID pShimData;
	PVOID AppCompatInfo;
	UNICODE_STRING CSDVersion;
	//_ACTIVATION_CONTEXT_DATA* ActivationContextData;
	//_ASSEMBLY_STORAGE_MAP* ProcessAssemblyStorageMap;
	//_ACTIVATION_CONTEXT_DATA* SystemDefaultActivationContextData;
	//_ASSEMBLY_STORAGE_MAP* SystemAssemblyStorageMap;
	ULONG MinimumStackCommit;
	//_FLS_CALLBACK_INFO* FlsCallback;
	LIST_ENTRY FlsListHead;
	PVOID FlsBitmap;
	ULONG FlsBitmapBits[4];
	ULONG FlsHighIndex;
	PVOID WerRegistrationData;
	PVOID WerShipAssertPtr;
} PEB, * PPEB;

typedef struct _POOL_HEADER
{
	union
	{
		struct
		{
#if defined(_AMD64_)
			ULONG	PreviousSize : 8;
			ULONG	PoolIndex : 8;
			ULONG	BlockSize : 8;
			ULONG	PoolType : 8;
#else
			USHORT	PreviousSize : 9;
			USHORT	PoolIndex : 7;
			USHORT	BlockSize : 9;
			USHORT	PoolType : 7;
#endif
		};
		ULONG	Ulong1;
	};
#if defined(_WIN64)
	ULONG	PoolTag;
#endif
	union
	{
#if defined(_WIN64)
		void* ProcessBilled;
#else
		ULONG	PoolTag;
#endif
		struct
		{
			USHORT	AllocatorBackTraceIndex;
			USHORT	PoolTagHash;
		};
	};
} POOL_HEADER, * PPOOL_HEADER;

typedef struct _OBJECT_HEADER
{
	LONG	PointerCount;
	union
	{
		LONG	HandleCount;
		PVOID	NextToFree;
	};
	uint64_t	Lock;
	UCHAR		TypeIndex;
	union
	{
		UCHAR	TraceFlags;
		struct
		{
			UCHAR	DbgRefTrace : 1;
			UCHAR	DbgTracePermanent : 1;
			UCHAR	Reserved : 6;
		};
	};
	UCHAR	InfoMask;
	union
	{
		UCHAR	Flags;
		struct
		{
			UCHAR	NewObject : 1;
			UCHAR	KernelObject : 1;
			UCHAR	KernelOnlyAccess : 1;
			UCHAR	ExclusiveObject : 1;
			UCHAR	PermanentObject : 1;
			UCHAR	DefaultSecurityQuota : 1;
			UCHAR	SingleHandleEntry : 1;
			UCHAR	DeletedInline : 1;
		};
	};
	union
	{
		PVOID	ObjectCreateInfo;
		PVOID	QuotaBlockCharged;
	};
	PVOID	SecurityDescriptor;
	PVOID	Body;
} OBJECT_HEADER, * POBJECT_HEADER;
struct PfnList {
	bool isPool;
};

template<typename SYS_TYPE>
std::unique_ptr<SYS_TYPE>
QueryInfo(__in SYSTEM_INFORMATION_CLASS sysClass
)
{
	size_t size = sizeof(RTL_PROCESS_MODULES) + SPAGE_SIZE;
	NTSTATUS status = STATUS_INFO_LENGTH_MISMATCH;
	void* info = malloc(size);
	if (!info)
		return std::unique_ptr<SYS_TYPE>(nullptr);

	for (; STATUS_INFO_LENGTH_MISMATCH == status; size *= 2)
	{
		status = NtQuerySystemInformation(
			(SYSTEM_INFORMATION_CLASS)sysClass,
			info,
			size,
			nullptr);

		info = realloc(info, size * 2);
		if (!info)
			break;
	}

	std::unique_ptr<SYS_TYPE> r_info = std::unique_ptr<SYS_TYPE>(static_cast<SYS_TYPE*>(info));
	return r_info;
}