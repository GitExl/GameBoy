import io
import binascii

def disasm(data, bankOffset=0):

	idx = 0;
	while idx < len(data):
		opcode = data[idx]

		print('{:X}: '.format(idx + bankOffset), end='')

		# NOP
		if opcode == 0x0:
			print('NOP')

		# 8-Bit loads
		# LD nn,n
		elif opcode == 0x06:
			print('LD   B, {:X}'.format(data[idx + 1]))
			idx += 1
		elif opcode == 0x0e:
			print('LD   C, {:X}'.format(data[idx + 1]))
			idx += 1
		elif opcode == 0x16:
			print('LD   D, {:X}'.format(data[idx + 1]))
			idx += 1
		elif opcode == 0x1e:
			print('LD   E, {:X}'.format(data[idx + 1]))
			idx += 1
		elif opcode == 0x26:
			print('LD   H, {:X}'.format(data[idx + 1]))
			idx += 1
		elif opcode == 0x2e:
			print('LD   L, {:X}'.format(data[idx + 1]))
			idx += 1

		# LD r1,r2
		elif opcode == 0x7f:
			print('LD   A, A')
		elif opcode == 0x78:
			print('LD   A, B')
		elif opcode == 0x79:
			print('LD   A, C')
		elif opcode == 0x7a:
			print('LD   A, D')
		elif opcode == 0x7b:
			print('LD   A, E')
		elif opcode == 0x7c:
			print('LD   A, H')
		elif opcode == 0x7d:
			print('LD   A, L')
		elif opcode == 0x7e:
			print('LD   A, (HL)')

		elif opcode == 0x40:
			print('LD   B, B')
		elif opcode == 0x41:
			print('LD   B, C')
		elif opcode == 0x42:
			print('LD   B, D')
		elif opcode == 0x43:
			print('LD   B, E')
		elif opcode == 0x44:
			print('LD   B, H')
		elif opcode == 0x45:
			print('LD   B, L')
		elif opcode == 0x46:
			print('LD   B, (HL)')

		elif opcode == 0x40:
			print('LD   B, B')
		elif opcode == 0x41:
			print('LD   B, C')
		elif opcode == 0x42:
			print('LD   B, D')
		elif opcode == 0x43:
			print('LD   B, E')
		elif opcode == 0x44:
			print('LD   B, H')
		elif opcode == 0x45:
			print('LD   B, L')
		elif opcode == 0x46:
			print('LD   B, (HL)')
		
		elif opcode == 0x48:
			print('LD   C, B')
		elif opcode == 0x49:
			print('LD   C, C')
		elif opcode == 0x4a:
			print('LD   C, D')
		elif opcode == 0x4b:
			print('LD   C, E')
		elif opcode == 0x4c:
			print('LD   C, H')
		elif opcode == 0x4d:
			print('LD   C, L')
		elif opcode == 0x4e:
			print('LD   C, (HL)')
		
		elif opcode == 0x50:
			print('LD   D, B')
		elif opcode == 0x51:
			print('LD   D, C')
		elif opcode == 0x52:
			print('LD   D, D')
		elif opcode == 0x53:
			print('LD   D, E')
		elif opcode == 0x54:
			print('LD   D, H')
		elif opcode == 0x55:
			print('LD   D, L')
		elif opcode == 0x56:
			print('LD   D, (HL)')
		
		elif opcode == 0x58:
			print('LD   E, B')
		elif opcode == 0x59:
			print('LD   E, C')
		elif opcode == 0x5A:
			print('LD   E, D')
		elif opcode == 0x5B:
			print('LD   E, E')
		elif opcode == 0x5C:
			print('LD   E, H')
		elif opcode == 0x5D:
			print('LD   E, L')
		elif opcode == 0x5E:
			print('LD   E, (HL)')
		
		elif opcode == 0x60:
			print('LD   H, B')
		elif opcode == 0x61:
			print('LD   H, C')
		elif opcode == 0x62:
			print('LD   H, D')
		elif opcode == 0x63:
			print('LD   H, E')
		elif opcode == 0x64:
			print('LD   H, H')
		elif opcode == 0x65:
			print('LD   H, L')
		elif opcode == 0x66:
			print('LD   H, (HL)')

		elif opcode == 0x68:
			print('LD   L, B')
		elif opcode == 0x69:
			print('LD   L, C')
		elif opcode == 0x6A:
			print('LD   L, D')
		elif opcode == 0x6B:
			print('LD   L, E')
		elif opcode == 0x6C:
			print('LD   L, H')
		elif opcode == 0x6D:
			print('LD   L, L')
		elif opcode == 0x6E:
			print('LD   L, (HL)')
		
		elif opcode == 0x70:
			print('LD   (HL), B')
		elif opcode == 0x71:
			print('LD   (HL), C')
		elif opcode == 0x72:
			print('LD   (HL), D')
		elif opcode == 0x73:
			print('LD   (HL), E')
		elif opcode == 0x74:
			print('LD   (HL), H')
		elif opcode == 0x75:
			print('LD   (HL), L')
		elif opcode == 0x36:
			print('LD   (HL), {:X}'.format(data[idx + 1]))
			idx += 1
		
		# LD A,n
		elif opcode == 0x0A:
			print('LD   A, (BC)')
		elif opcode == 0x1A:
			print('LD   A, (DE)')
		elif opcode == 0x7E:
			print('LD   A, (HL)')
		elif opcode == 0xFA:
			print('LD   A, ({:X}{:X})'.format(data[idx + 2], data[idx + 1]))
			idx += 2
		elif opcode == 0x3E:
			print('LD   A, {:X}'.format(data[idx + 1]))
			idx += 1

		# LD n,A
		elif opcode == 0x7F:
			print('LD   A, A')
		elif opcode == 0x47:
			print('LD   B, A')
		elif opcode == 0x4F:
			print('LD   C, A')
		elif opcode == 0x57:
			print('LD   D, A')
		elif opcode == 0x5F:
			print('LD   E, A')
		elif opcode == 0x67:
			print('LD   H, A')
		elif opcode == 0x6F:
			print('LD   L, A')
		elif opcode == 0x02:
			print('LD   (BC), A')
		elif opcode == 0x12:
			print('LD   (DE), A')
		elif opcode == 0x77:
			print('LD   (HL), A')
		elif opcode == 0xEA:
			print('LD   ({:X}{:X}), A'.format(data[idx + 2], data[idx + 1]))
			idx += 2

		# LD A,(C)
		elif opcode == 0xF2:
			print('LD  A, (FF00 + C)')

		# LD (C),A
		elif opcode == 0xE2:
			print('LD  (FF00 + C), A')

		# LDD A,(HL)
		elif opcode == 0x3A:
			print('LDD  A, (HL)')

		# LDD (HL),A
		elif opcode == 0x32:
			print('LDD  (HL), A')

		# LDI A,(HL)
		elif opcode == 0x2A:
			print('LDI  A, (HL)')

		# LDI (HL),A
		elif opcode == 0x22:
			print('LDI  (HL) A')

		# LDH (n),A
		elif opcode == 0xE0:
			print('LDH  (FF00 + {:X}), A'.format(data[idx + 1]))
			idx += 1

		# LDH A,(n)
		elif opcode == 0xF0:
			print('LDH  A, (FF00 + {:X})'.format(data[idx + 1]))
			idx += 1

		# 16-Bit loads
		# LD n,nn
		elif opcode == 0x01:
			print('LD   BC, {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2
		elif opcode == 0x11:
			print('LD   DE, {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2
		elif opcode == 0x21:
			print('LD   HL, {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2
		elif opcode == 0x31:
			print('LD   SP, {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2

		# LD SP,HL
		elif opcode == 0xF9:
			print('LD   SP, HL')

		# LDHL SP,n
		elif opcode == 0xF8:
			print('LDHL SP, {:X}'.format(data[idx + 1]))
			idx += 1

		# LD (nn),SP
		elif opcode == 0x08:
			print('LD   ({:X}{:X}), SP'.format(data[idx + 2], data[idx + 1]))
			idx += 2

		# PUSH nn
		elif opcode == 0xF5:
			print('PUSH AF')
		elif opcode == 0xC5:
			print('PUSH BC')
		elif opcode == 0xD5:
			print('PUSH DE')
		elif opcode == 0xE5:
			print('PUSH HL')
		
		# POP nn
		elif opcode == 0xF1:
			print('POP  AF')
		elif opcode == 0xC1:
			print('POP  BC')
		elif opcode == 0xD1:
			print('POP  DE')
		elif opcode == 0xE1:
			print('POP  HL')

		# 8-Bit ALU
		# ADD A,n
		elif opcode == 0x87:
			print('ADD  A, A')
		elif opcode == 0x80:
			print('ADD  A, B')
		elif opcode == 0x81:
			print('ADD  A, C')
		elif opcode == 0x82:
			print('ADD  A, D')
		elif opcode == 0x83:
			print('ADD  A, E')
		elif opcode == 0x84:
			print('ADD  A, H')
		elif opcode == 0x85:
			print('ADD  A, L')
		elif opcode == 0x86:
			print('ADD  A, (HL)')
		elif opcode == 0xC6:
			print('ADD  A, {:X}'.format(data[idx + 1]))
			idx += 1

		# ADC A,n
		elif opcode == 0x8F:
			print('ADC  A, A')
		elif opcode == 0x88:
			print('ADC  A, B')
		elif opcode == 0x89:
			print('ADC  A, C')
		elif opcode == 0x8A:
			print('ADC  A, D')
		elif opcode == 0x8B:
			print('ADC  A, E')
		elif opcode == 0x8C:
			print('ADC  A, H')
		elif opcode == 0x8D:
			print('ADC  A, L')
		elif opcode == 0x8E:
			print('ADC  A, (HL)')
		elif opcode == 0xCE:
			print('ADC  A, {:X}'.format(data[idx + 1]))
			idx += 1

		# SUB A,n
		elif opcode == 0x97:
			print('SUB  A, A')
		elif opcode == 0x90:
			print('SUB  A, B')
		elif opcode == 0x91:
			print('SUB  A, C')
		elif opcode == 0x92:
			print('SUB  A, D')
		elif opcode == 0x93:
			print('SUB  A, E')
		elif opcode == 0x94:
			print('SUB  A, H')
		elif opcode == 0x95:
			print('SUB  A, L')
		elif opcode == 0x96:
			print('SUB  A, (HL)')
		elif opcode == 0xD6:
			print('SUB  A, {:X}'.format(data[idx + 1]))
			idx += 1

		# SBC A,n
		elif opcode == 0x9F:
			print('SBC  A, A')
		elif opcode == 0x98:
			print('SBC  A, B')
		elif opcode == 0x99:
			print('SBC  A, C')
		elif opcode == 0x9A:
			print('SBC  A, D')
		elif opcode == 0x9B:
			print('SBC  A, E')
		elif opcode == 0x9C:
			print('SBC  A, H')
		elif opcode == 0x9D:
			print('SBC  A, L')
		elif opcode == 0x9E:
			print('SBC  A, (HL)')
		elif opcode == 0xDE:
			print('SBC  A, {:X}'.format(data[idx + 1]))
			idx += 1

		# AND n
		elif opcode == 0xA7:
			print('AND  A, A')
		elif opcode == 0xA0:
			print('AND  A, B')
		elif opcode == 0xA1:
			print('AND  A, C')
		elif opcode == 0xA2:
			print('AND  A, D')
		elif opcode == 0xA3:
			print('AND  A, E')
		elif opcode == 0xA4:
			print('AND  A, H')
		elif opcode == 0xA5:
			print('AND  A, L')
		elif opcode == 0xA6:
			print('AND  A, (HL)')
		elif opcode == 0xE6:
			print('AND  A, {:X}'.format(data[idx + 1]))
			idx += 1
		
		# OR n
		elif opcode == 0xB7:
			print('OR   A, A')
		elif opcode == 0xB0:
			print('OR   A, B')
		elif opcode == 0xB1:
			print('OR   A, C')
		elif opcode == 0xB2:
			print('OR   A, D')
		elif opcode == 0xB3:
			print('OR   A, E')
		elif opcode == 0xB4:
			print('OR   A, H')
		elif opcode == 0xB5:
			print('OR   A, L')
		elif opcode == 0xB6:
			print('OR   A, (HL)')
		elif opcode == 0xF6:
			print('OR   A, {:X}'.format(data[idx + 1]))
			idx += 1
		
		# XOR n
		elif opcode == 0xAF:
			print('XOR  A, A')
		elif opcode == 0xA8:
			print('XOR  A, B')
		elif opcode == 0xA9:
			print('XOR  A, C')
		elif opcode == 0xAA:
			print('XOR  A, D')
		elif opcode == 0xAB:
			print('XOR  A, E')
		elif opcode == 0xAC:
			print('XOR  A, H')
		elif opcode == 0xAD:
			print('XOR  A, L')
		elif opcode == 0xAE:
			print('XOR  A, (HL)')
		elif opcode == 0xEE:
			print('XOR  A, {:X}'.format(data[idx + 1]))
			idx += 1

		# CP n
		elif opcode == 0xBF:
			print('CP   A, A')
		elif opcode == 0xB8:
			print('CP   A, B')
		elif opcode == 0xB9:
			print('CP   A, C')
		elif opcode == 0xBA:
			print('CP   A, D')
		elif opcode == 0xBB:
			print('CP   A, E')
		elif opcode == 0xBC:
			print('CP   A, H')
		elif opcode == 0xBD:
			print('CP   A, L')
		elif opcode == 0xBE:
			print('CP   A, (HL)')
		elif opcode == 0xFE:
			print('CP   A, {:X}'.format(data[idx + 1]))
			idx += 1
		
		# INC n
		elif opcode == 0x3C:
			print('INC  A')
		elif opcode == 0x04:
			print('INC  B')
		elif opcode == 0x0C:
			print('INC  C')
		elif opcode == 0x14:
			print('INC  D')
		elif opcode == 0x1C:
			print('INC  E')
		elif opcode == 0x24:
			print('INC  H')
		elif opcode == 0x2C:
			print('INC  L')
		elif opcode == 0x34:
			print('INC  (HL)')
		
		# DEC n
		elif opcode == 0x3D:
			print('DEC  A')
		elif opcode == 0x05:
			print('DEC  B')
		elif opcode == 0x0D:
			print('DEC  C')
		elif opcode == 0x15:
			print('DEC  D')
		elif opcode == 0x1D:
			print('DEC  E')
		elif opcode == 0x25:
			print('DEC  H')
		elif opcode == 0x2D:
			print('DEC  L')
		elif opcode == 0x35:
			print('DEC  (HL)')

		# 16-Bit arithmetic
		# ADD HL,n
		elif opcode == 0x09:
			print('ADD  HL, BC')
		elif opcode == 0x19:
			print('ADD  HL, DE')
		elif opcode == 0x29:
			print('ADD  HL, HL')
		elif opcode == 0x39:
			print('ADD  HL, SP')

		# ADD SP,n
		elif opcode == 0xE8:
			print('ADD  SP, {:X}'.format(data[idx + 1]))
			idx += 1

		# INC nn
		elif opcode == 0x03:
			print('INC  BC')
		elif opcode == 0x13:
			print('INC  DE')
		elif opcode == 0x23:
			print('INC  HL')
		elif opcode == 0x33:
			print('INC  SP')

		# DEC nn
		elif opcode == 0x0B:
			print('DEC  BC')
		elif opcode == 0x1B:
			print('DEC  DE')
		elif opcode == 0x2B:
			print('DEC  HL')
		elif opcode == 0x3B:
			print('DEC  SP')

		# Miscellaneous
		# DAA
		elif opcode == 0x27:
			print('DAA')

		# CPL
		elif opcode == 0x2F:
			print('CPL')

		# CCF
		elif opcode == 0x3F:
			print('CCF')

		# SCF
		elif opcode == 0x37:
			print('SCF')

		# HALT
		elif opcode == 0x76:
			print('HALT')

		# STOP
		elif opcode == 0x10:
			print('STOP')

		# DI
		elif opcode == 0xF3:
			print('DI')
		
		# EI
		elif opcode == 0xFB:
			print('EI')

		# Rotates and shifts
		# RCLA
		elif opcode == 0x07:
			print('RLCA')

		# RLA
		elif opcode == 0x17:
			print('RLA')

		# RRCA
		elif opcode == 0x0F:
			print('RRCA')

		# RRA
		elif opcode == 0x1F:
			print('RRA')

		# Jumps
		# JP nn
		elif opcode == 0xC3:
			print('JP   {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2
		
		# JP cc,nn
		elif opcode == 0xC2:
			print('JP   NZ, {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2
		elif opcode == 0xCA:
			print('JP   Z, {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2
		elif opcode == 0xD2:
			print('JP   NC, {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2
		elif opcode == 0xDA:
			print('JP   C, {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2
		
		# JP HL
		elif opcode == 0xE9:
			print('JP   (HL)')
		
		# JR n
		elif opcode == 0x18:
			print('JR   {:X}'.format(data[idx + 1] - 256))
			idx += 1

		# JR cc,n
		elif opcode == 0x20:
			print('JR   NZ, {:X}'.format(data[idx + 1] - 256))
			idx += 1
		elif opcode == 0x28:
			print('JR   Z, {:X}'.format(data[idx + 1] - 256))
			idx += 1
		elif opcode == 0x30:
			print('JR   NC, {:X}'.format(data[idx + 1] - 256))
			idx += 1
		elif opcode == 0x38:
			print('JR   C, {:X}'.format(data[idx + 1] - 256))
			idx += 1

		# Calls
		# CALL nn
		elif opcode == 0xCD:
			print('CALL {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2

		# CALL cc,nn
		elif opcode == 0xC4:
			print('CALL NZ, {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2
		elif opcode == 0xCC:
			print('CALL Z, {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2
		elif opcode == 0xD4:
			print('CALL NC, {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2
		elif opcode == 0xDC:
			print('CALL C, {:X}{:X}'.format(data[idx + 2], data[idx + 1]))
			idx += 2

		# Restarts
		# RST n
		elif opcode == 0xC7:
			print('RST  00')
		elif opcode == 0xCF:
			print('RST  08')
		elif opcode == 0xD7:
			print('RST  10')
		elif opcode == 0xDF:
			print('RST  18')
		elif opcode == 0xE7:
			print('RST  20')
		elif opcode == 0xEF:
			print('RST  28')
		elif opcode == 0xF7:
			print('RST  30')
		elif opcode == 0xFF:
			print('RST  38')

		# Returns
		# RET
		elif opcode == 0xC9:
			print('RET')

		# RET cc
		elif opcode == 0xC0:
			print('RET  NZ')
		elif opcode == 0xC8:
			print('RET  Z')
		elif opcode == 0xD0:
			print('RET  NC')
		elif opcode == 0xD8:
			print('RET  C')

		# RETI
		elif opcode == 0xD9:
			print('RETI')
			
		# 0xCB subset
		elif opcode == 0xCB:
			idx += 1
			opcode = data[idx]
			
			# Rotates and shifts
			# RLC n
			if opcode == 0x07:
				print('RLC  A')
			elif opcode == 0x00:
				print('RLC  B')
			elif opcode == 0x01:
				print('RLC  C')
			elif opcode == 0x02:
				print('RLC  D')
			elif opcode == 0x03:
				print('RLC  E')
			elif opcode == 0x04:
				print('RLC  H')
			elif opcode == 0x05:
				print('RLC  L')
			elif opcode == 0x06:
				print('RLC  (HL)')

			# RL n
			elif opcode == 0x17:
				print('RL   A')
			elif opcode == 0x10:
				print('RL   B')
			elif opcode == 0x11:
				print('RL   C')
			elif opcode == 0x12:
				print('RL   D')
			elif opcode == 0x13:
				print('RL   E')
			elif opcode == 0x14:
				print('RL   H')
			elif opcode == 0x15:
				print('RL   L')
			elif opcode == 0x16:
				print('RL   (HL)')

			# RRC n
			elif opcode == 0x0F:
				print('RRC  A')
			elif opcode == 0x08:
				print('RRC  B')
			elif opcode == 0x09:
				print('RRC  C')
			elif opcode == 0x0A:
				print('RRC  D')
			elif opcode == 0x0B:
				print('RRC  E')
			elif opcode == 0x0C:
				print('RRC  H')
			elif opcode == 0x0D:
				print('RRC  L')
			elif opcode == 0x0E:
				print('RRC  (HL)')

			# RR n
			elif opcode == 0x1F:
				print('RR   A')
			elif opcode == 0x18:
				print('RR   B')
			elif opcode == 0x19:
				print('RR   C')
			elif opcode == 0x1A:
				print('RR   D')
			elif opcode == 0x1B:
				print('RR   E')
			elif opcode == 0x1C:
				print('RR   H')
			elif opcode == 0x1D:
				print('RR   L')
			elif opcode == 0x1E:
				print('RR   (HL)')

			# SLA n
			elif opcode == 0x27:
				print('SLA  A')
			elif opcode == 0x20:
				print('SLA  B')
			elif opcode == 0x21:
				print('SLA  C')
			elif opcode == 0x22:
				print('SLA  D')
			elif opcode == 0x23:
				print('SLA  E')
			elif opcode == 0x24:
				print('SLA  H')
			elif opcode == 0x25:
				print('SLA  L')
			elif opcode == 0x26:
				print('SLA  (HL)')

			# SRA n
			elif opcode == 0x2F:
				print('SRA  A')
			elif opcode == 0x28:
				print('SRA  B')
			elif opcode == 0x29:
				print('SRA  C')
			elif opcode == 0x2A:
				print('SRA  D')
			elif opcode == 0x2B:
				print('SRA  E')
			elif opcode == 0x2C:
				print('SRA  H')
			elif opcode == 0x2D:
				print('SRA  L')
			elif opcode == 0x2E:
				print('SRA  (HL)')

			# SRL n
			elif opcode == 0x3F:
				print('SRL  A')
			elif opcode == 0x38:
				print('SRL  B')
			elif opcode == 0x39:
				print('SRL  C')
			elif opcode == 0x3A:
				print('SRL  D')
			elif opcode == 0x3B:
				print('SRL  E')
			elif opcode == 0x3C:
				print('SRL  H')
			elif opcode == 0x3D:
				print('SRL  L')
			elif opcode == 0x3E:
				print('SRL  (HL)')

			# Bit opcodes
			# BIT b,r
			elif opcode == 0x47:
				print('BIT  {}, A'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x40:
				print('BIT  {}, B'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x41:
				print('BIT  {}, C'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x42:
				print('BIT  {}, D'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x43:
				print('BIT  {}, E'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x44:
				print('BIT  {}, H'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x45:
				print('BIT  {}, L'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x46:
				print('BIT  {}, (HL)'.format(data[idx + 1]))
				idx += 1

			# SET b,r
			elif opcode == 0xC7:
				print('SET  {}, A'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0xC0:
				print('SET  {}, B'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0xC1:
				print('SET  {}, C'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0xC2:
				print('SET  {}, D'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0xC3:
				print('SET  {}, E'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0xC4:
				print('SET  {}, H'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0xC5:
				print('SET  {}, L'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0xC6:
				print('SET  {}, (HL)'.format(data[idx + 1]))
				idx += 1

			# RES b,r
			elif opcode == 0x87:
				print('RES  {}, A'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x80:
				print('RES  {}, B'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x81:
				print('RES  {}, C'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x82:
				print('RES  {}, D'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x83:
				print('RES  {}, E'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x84:
				print('RES  {}, H'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x85:
				print('RES  {}, L'.format(data[idx + 1]))
				idx += 1
			elif opcode == 0x86:
				print('RES  {}, (HL)'.format(data[idx + 1]))
				idx += 1

			# Miscellaneous
			# SWAP n
			elif opcode == 0x37:
				print('SWAP A')
			elif opcode == 0x30:
				print('SWAP B')
			elif opcode == 0x31:
				print('SWAP C')
			elif opcode == 0x32:
				print('SWAP D')
			elif opcode == 0x33:
				print('SWAP E')
			elif opcode == 0x34:
				print('SWAP H')
			elif opcode == 0x35:
				print('SWAP L')
			elif opcode == 0x36:
				print('SWAP (HL)')

			else:
				print('Unknown 0xCB opcode {:X}'.format(opcode))

		else:
			print('Unknown opcode {:X}'.format(opcode))
		
		idx += 1


if __name__ == "__main__":

	d = [0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
    0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
    0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
    0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
    0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
    0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
    0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
    0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
    0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
    0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
    0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
    0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
    0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
    0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
    0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50]

	disasm(d)
	exit

	file = io.open('Tetris.gb', mode='rb')
	
	file.seek(0x100)
	header = file.read(0x50)

	print('** HEADER **')
	print('Name                : {}'.format(header[0x34:0x43].decode('ascii')))
	print('Manufacturer (old)  : {}'.format(header[0x3F:0x42].decode('ascii')))
	print('Licensee code (new) : {:X}{:X}'.format(header[0x44], header[0x45]))
	print('Licensee code (old) : {:X}'.format(header[0x4B]))
	print('Version             : {}'.format(header[0x4C]))
	print('Region              : {}'.format(header[0x4A]))
	print('Cartridge type      : {}'.format(header[0x47]))
	print('ROM size            : {} KByte'.format((32768 << header[0x48]) / 1024))
	print('RAM size            : {}'.format(header[0x49]))
	print('Support CGB         : {:X}'.format(header[0x43]))
	print('Support SGB         : {:X}'.format(header[0x46]))
	print('Header checksum     : {:X}'.format(header[0x4D]))
	print('Total checksum      : {:X}{:X}'.format(header[0x4E], header[0x4F]))
	print()

	bankcount = int(((32768 << header[0x48]) / 1024) / 16)
	for x in range(bankcount):
		file.seek(x * 0x4000)
		data = file.read(0x4000);
		print('** BANK {} **'.format(x))
		
		if x == 0:
			disasm(data)
		else:
			disasm(data, 0x4000)
		print()

	file.close()