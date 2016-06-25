#include "stdafx.h"
#include "Emu/Memory/Memory.h"
#include "Emu/System.h"

#include "Emu/Cell/ErrorCodes.h"
#include "sys_rsx.h"

logs::channel sys_rsx("sys_rsx", logs::level::notice);

s32 sys_rsx_device_open()
{
	sys_rsx.todo("sys_rsx_device_open()");

	return CELL_OK;
}

s32 sys_rsx_device_close()
{
	sys_rsx.todo("sys_rsx_device_close()");

	return CELL_OK;
}

/*
 * lv2 SysCall 668 (0x29C): sys_rsx_memory_allocate
 * @param mem_handle (OUT): Context / ID, which is used by sys_rsx_memory_free to free allocated memory.
 * @param mem_addr (OUT): Returns the local memory base address, usually 0xC0000000.
 * @param size (IN): Local memory size. E.g. 0x0F900000 (249 MB).
 * @param flags (IN): E.g. Immediate value passed in cellGcmSys is 8.
 * @param a5 (IN): E.g. Immediate value passed in cellGcmSys is 0x00300000 (3 MB?).
 * @param a6 (IN): E.g. Immediate value passed in cellGcmSys is 16. 
 * @param a7 (IN): E.g. Immediate value passed in cellGcmSys is 8.
 */
s32 sys_rsx_memory_allocate(vm::ptr<u32> mem_handle, vm::ptr<u64> mem_addr, u32 size, u64 flags, u64 a5, u64 a6, u64 a7)
{
	sys_rsx.todo("sys_rsx_memory_allocate(mem_handle=*0x%x, mem_addr=*0x%x, size=0x%x, flags=0x%llx, a5=0x%llx, a6=0x%llx, a7=0x%llx)", mem_handle, mem_addr, size, flags, a5, a6, a7);

	return CELL_OK;
}

/*
 * lv2 SysCall 669 (0x29D): sys_rsx_memory_free
 * @param mem_handle (OUT): Context / ID, for allocated local memory generated by sys_rsx_memory_allocate
 */
s32 sys_rsx_memory_free(u32 mem_handle)
{
	sys_rsx.todo("sys_rsx_memory_free(mem_handle=0x%x)", mem_handle);

	return CELL_OK;
}

/*
 * lv2 SysCall 670 (0x29E): sys_rsx_context_allocate
 * @param context_id (OUT): RSX context, E.g. 0x55555555 (in vsh.self)
 * @param lpar_dma_control (OUT): Control register area. E.g. 0x60100000 (in vsh.self)
 * @param lpar_driver_info (OUT): RSX data like frequencies, sizes, version... E.g. 0x60200000 (in vsh.self)
 * @param lpar_reports (OUT): Report data area. E.g. 0x60300000 (in vsh.self)
 * @param mem_ctx (IN): mem_ctx given by sys_rsx_memory_allocate
 * @param system_mode (IN):
 */
s32 sys_rsx_context_allocate(vm::ptr<u32> context_id, vm::ptr<u32> lpar_dma_control, vm::ptr<u32> lpar_driver_info, vm::ptr<u32> lpar_reports, u64 mem_ctx, u64 system_mode)
{
	sys_rsx.todo("sys_rsx_context_allocate(context_id=*0x%x, lpar_dma_control=*0x%x, lpar_driver_info=*0x%x, lpar_reports=*0x%x, mem_ctx=0x%llx, system_mode=0x%llx)",
		context_id, lpar_dma_control, lpar_driver_info, lpar_reports, mem_ctx, system_mode);

	return CELL_OK;
}

/*
 * lv2 SysCall 671 (0x29F): sys_rsx_context_free
 * @param context_id (IN): RSX context generated by sys_rsx_context_allocate to free the context.
 */
s32 sys_rsx_context_free(u32 context_id)
{
	sys_rsx.todo("sys_rsx_context_free(context_id=0x%x)", context_id);

	return CELL_OK;
}

/*
 * lv2 SysCall 672 (0x2A0): sys_rsx_context_iomap
 * @param context_id (IN): RSX context, E.g. 0x55555555 (in vsh.self)
 * @param io (IN): IO offset mapping area. E.g. 0x00600000
 * @param ea (IN): Start address of mapping area. E.g. 0x20400000
 * @param size (IN): Size of mapping area in bytes. E.g. 0x00200000
 * @param flags (IN):
 */
s32 sys_rsx_context_iomap(u32 context_id, u32 io, u32 ea, u32 size, u64 flags)
{
	sys_rsx.todo("sys_rsx_context_iomap(context_id=0x%x, io=0x%x, ea=0x%x, size=0x%x, flags=0x%llx)", context_id, io, ea, size, flags);

	return CELL_OK;
}

/*
 * lv2 SysCall 673 (0x2A1): sys_rsx_context_iounmap
 * @param context_id (IN): RSX context, E.g. 0x55555555 (in vsh.self)
 * @param a2 (IN): ?
 * @param io_addr (IN): IO address. E.g. 0x00600000 (Start page 6)
 * @param size (IN): Size to unmap in byte. E.g. 0x00200000
 */
s32 sys_rsx_context_iounmap(u32 context_id, u32 a2, u32 io_addr, u32 size)
{
	sys_rsx.todo("sys_rsx_context_iounmap(context_id=0x%x, a2=0x%x, io_addr=0x%x, size=0x%x)", context_id, a2, io_addr, size);

	return CELL_OK;
}

/*
 * lv2 SysCall 674 (0x2A2): sys_rsx_context_attribute
 * @param context_id (IN): RSX context, e.g. 0x55555555
 * @param package_id (IN): 
 * @param a3 (IN): 
 * @param a4 (IN): 
 * @param a5 (IN): 
 * @param a6 (IN): 
 */
s32 sys_rsx_context_attribute(s32 context_id, u32 package_id, u64 a3, u64 a4, u64 a5, u64 a6)
{
	sys_rsx.todo("sys_rsx_context_attribute(context_id=0x%x, package_id=0x%x, a3=0x%llx, a4=0x%llx, a5=0x%llx, a6=0x%llx)", context_id, package_id, a3, a4, a5, a6);

	switch(package_id)
	{
	case 0x001: // FIFO
		break;
	
	case 0x100: // Display mode set
		break;

	case 0x101: // Display sync
		break;

	case 0x102: // Display flip
		break;

	case 0x103: // ?
		break;

	case 0x104: // Display buffer
		break;

	case 0x106: // ? (Used by cellGcmInitPerfMon)
		break;

	case 0x10a: // ?
		break;

	case 0x300: // Tiles
		break;

	case 0x301: // Depth-buffer (Z-cull)
		break;

	case 0x600: // Framebuffer setup
		break;

	case 0x601: // Framebuffer blit
		break;

	case 0x602: // Framebuffer blit sync
		break;

	default:
		return CELL_EINVAL;
	}

	return CELL_OK;
}

/*
 * lv2 SysCall 675 (0x2A3): sys_rsx_device_map
 * @param a1 (OUT): For example: In vsh.self it is 0x60000000, global semaphore. For a game it is 0x40000000.
 * @param a2 (OUT): Unused?
 * @param dev_id (IN): An immediate value and always 8. (cellGcmInitPerfMon uses 11, 10, 9, 7, 12 successively).
 */
s32 sys_rsx_device_map(vm::ptr<u32> addr, vm::ptr<u32> a2, u32 dev_id)
{
	sys_rsx.todo("sys_rsx_device_map(addr=*0x%x, a2=0x%x, dev_id=0x%x)", addr, a2, dev_id);

	if (dev_id > 15) {
		// TODO: Throw RSX error
		return CELL_EINVAL;
	}

	if (dev_id == 0 || dev_id > 8) {
		// TODO: lv1 related so we may ignore it.
		// if (something) { return CELL_EPERM; }
	}

	return CELL_OK;
}

/*
 * lv2 SysCall 676 (0x2A4): sys_rsx_device_unmap
 * @param dev_id (IN): An immediate value and always 8.
 */
s32 sys_rsx_device_unmap(u32 dev_id)
{
	sys_rsx.todo("sys_rsx_device_unmap(dev_id=0x%x)", dev_id);

	return CELL_OK;
}

s32 sys_rsx_attribute(u32 a1, u32 a2, u32 a3, u32 a4, u32 a5)
{
	sys_rsx.todo("sys_rsx_attribute(a1=0x%x, a2=0x%x, a3=0x%x, a4=0x%x, a5=0x%x)", a1, a2, a3, a4, a5);

	return CELL_OK;
}
