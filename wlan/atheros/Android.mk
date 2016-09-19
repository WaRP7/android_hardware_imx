ifneq ($(strip $(TARGET_NO_KERNEL)),true)
	include $(call all-subdir-makefiles)
endif

